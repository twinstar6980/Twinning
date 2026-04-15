#pragma once

#include <ShlObj_core.h>
#include <Windows.h>
#include <winrt/base.h>
#include <winrt/windows.applicationmodel.h>
#include <flutter/flutter_view_controller.h>
#include <flutter/method_channel.h>
#include <flutter/standard_method_codec.h>
#include "./flutter_window.h"
#include "./common.hpp"

class CustomMethodChannel {

private:

	FlutterWindow * m_host;

public:

	#pragma region construct

	~CustomMethodChannel(
	) = default;

	// ----------------

	CustomMethodChannel(
	) = delete;

	CustomMethodChannel(
		CustomMethodChannel const & that
	) = delete;

	CustomMethodChannel(
		CustomMethodChannel && that
	) = delete;

	// ----------------

	explicit CustomMethodChannel(
		FlutterWindow * host
	) :
		m_host{host} {
		return;
	}

	#pragma endregion

	#pragma region operator

	auto operator =(
		CustomMethodChannel const & that
	) -> CustomMethodChannel & = delete;

	auto operator =(
		CustomMethodChannel && that
	) -> CustomMethodChannel & = delete;

	#pragma endregion

public:

	#pragma region register

	// ReSharper disable once CppInconsistentNaming
	auto register_OnCreate(
	) -> void {
		auto channel = flutter::MethodChannel<>{
			thiz.m_host->flutter_controller_->engine()->messenger(),
			std::format("{}.CustomMethodChannel", thiz.get_application_identifier()),
			&flutter::StandardMethodCodec::GetInstance(),
		};
		channel.SetMethodCallHandler(
			[&](
			flutter::MethodCall<> const &            call,
			std::unique_ptr<flutter::MethodResult<>> result
		) -> void {
				thiz.handle(call, std::move(result));
				return;
			}
		);
		return;
	}

	#pragma endregion

private:

	#pragma region handle

	auto handle(
		flutter::MethodCall<> const &            call,
		std::unique_ptr<flutter::MethodResult<>> result
	) -> void {
		try {
			auto get_argument = [&]<typename TValue>(std::string_view const & name) -> TValue {
				auto map = std::get<flutter::EncodableMap>(*call.arguments());
				auto item = map.find(flutter::EncodableValue{name.data()});
				assert_test(item != map.end());
				return std::get<TValue>(item->second);
			};
			auto set_result = [&]<auto t_optional, typename TValue>(std::string_view const & name, TValue && value) -> std::pair<flutter::EncodableValue, flutter::EncodableValue> {
				auto result_value = flutter::EncodableValue{};
				if constexpr (t_optional) {
					if (value.has_value()) {
						result_value.emplace<typename std::decay_t<TValue>::value_type>(value.value());
					}
				}
				else {
					result_value.emplace<std::decay_t<TValue>>(std::forward<TValue>(value));
				}
				return std::make_tuple(flutter::EncodableValue{name.data()}, result_value);
			};
			switch (thiz.hash_string(call.method_name())) {
				case thiz.hash_string("query_application_shared_path"): {
					auto detail = thiz.handle_query_application_shared_path(
					);
					result->Success(
						flutter::EncodableMap{{
							set_result.operator ()<false>("path", std::get<0>(detail)),
						}}
					);
					break;
				}
				case thiz.hash_string("pick_storage_item"): {
					auto detail = thiz.handle_pick_storage_item(
						get_argument.operator ()<std::string>("type"),
						get_argument.operator ()<std::string>("location"),
						get_argument.operator ()<std::string>("name")
					);
					result->Success(
						flutter::EncodableMap{{
							set_result.operator ()<true>("target", std::get<0>(detail)),
						}}
					);
					break;
				}
				default: {
					result->NotImplemented();
					break;
				}
			}
		}
		catch (...) {
			result->Error("", thiz.parse_current_exception());
		}
		return;
	}

	auto handle_query_application_shared_path(
	) -> std::tuple<std::string> {
		return std::make_tuple(thiz.get_application_shared_directory());
	}

	auto handle_pick_storage_item(
		std::string const & type,
		std::string const & location,
		std::string const & name
	) -> std::tuple<std::optional<std::string>> {
		assert_test(type == "load_file" || type == "load_directory" || type == "save_file");
		auto state_h = HRESULT{};
		auto location_h = winrt::to_hstring(location);
		auto name_h = winrt::to_hstring(name);
		auto target = std::optional<std::string>{};
		auto dialog = winrt::com_ptr<IFileDialog>{};
		if (type == "load_file" || type == "load_directory") {
			state_h = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(dialog.put()));
			winrt::check_hresult(state_h);
		}
		if (type == "save_file") {
			state_h = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(dialog.put()));
			winrt::check_hresult(state_h);
		}
		auto option = FILEOPENDIALOGOPTIONS{};
		option |= FOS_NOCHANGEDIR;
		option |= FOS_NODEREFERENCELINKS;
		option |= FOS_FORCESHOWHIDDEN;
		if (type == "load_file" || type == "load_directory") {
			option |= FOS_PATHMUSTEXIST;
			option |= FOS_FILEMUSTEXIST;
		}
		if (type == "load_directory") {
			option |= FOS_PICKFOLDERS;
		}
		if (type == "save_file") {
			option |= FOS_PATHMUSTEXIST;
			option |= FOS_OVERWRITEPROMPT;
			option |= FOS_NOREADONLYRETURN;
		}
		state_h = dialog->SetOptions(option);
		winrt::check_hresult(state_h);
		auto location_item = winrt::com_ptr<IShellItem>{};
		state_h = SHCreateItemFromParsingName(location_h.data(), nullptr, IID_PPV_ARGS(location_item.put()));
		winrt::check_hresult(state_h);
		state_h = dialog->SetFolder(location_item.get());
		winrt::check_hresult(state_h);
		if (type == "save_file") {
			state_h = dialog->SetFileName(name_h.data());
			winrt::check_hresult(state_h);
		}
		state_h = dialog->Show(thiz.m_host->GetHandle());
		if (state_h != HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
			winrt::check_hresult(state_h);
			auto target_item = winrt::com_ptr<IShellItem>{};
			state_h = dialog->GetResult(target_item.put());
			winrt::check_hresult(state_h);
			target = thiz.get_shell_item_path(target_item);
		}
		return std::make_tuple(target);
	}

	#pragma endregion

	#pragma region utility

	auto get_application_identifier(
	) const -> std::string {
		auto identifier = std::string{};
		try {
			identifier = winrt::to_string(winrt::Windows::ApplicationModel::Package::Current().Id().Name());
		}
		catch (...) {
			identifier = "com.twinstar.twinning.assistant";
		}
		return identifier;
	}

	auto get_application_shared_directory(
	) const -> std::string {
		auto state_h = HRESULT{};
		auto parent_p = LPWSTR{};
		state_h = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &parent_p);
		winrt::check_hresult(state_h);
		auto parent_h = std::wstring{parent_p};
		CoTaskMemFree(parent_p);
		return std::format("{}\\{}", winrt::to_string(parent_h), thiz.get_application_identifier());
	}

	// ----------------

	auto parse_current_exception(
	) const -> std::string {
		auto exception = std::current_exception();
		auto message = std::string{};
		try {
			std::rethrow_exception(exception);
		}
		catch (std::exception & e) {
			message = e.what();
		}
		catch (winrt::hresult_error & e) {
			message = winrt::to_string(e.message());
		}
		catch (...) {
			message = "UnknownException";
		}
		return message;
	}

	// ----------------

	auto get_file_long_path(
		std::string const & source
	) const -> std::string {
		auto state_d = DWORD{};
		auto source_h = winrt::to_hstring(source);
		auto destination_h = std::wstring{};
		if (!source.empty()) {
			state_d = GetLongPathNameW(source_h.data(), destination_h.data(), 0);
			assert_test(state_d != 0);
			destination_h.resize(state_d);
			state_d = GetLongPathNameW(source_h.data(), destination_h.data(), static_cast<DWORD>(destination_h.size()));
			assert_test(state_d == destination_h.size() - 1);
			destination_h.resize(destination_h.size() - 1);
		}
		return winrt::to_string(destination_h);
	}

	auto get_shell_item_path(
		winrt::com_ptr<IShellItem> const & item
	) const -> std::string {
		auto state_h = HRESULT{};
		auto item_display = LPWSTR{};
		state_h = item->GetDisplayName(SIGDN_FILESYSPATH, &item_display);
		winrt::check_hresult(state_h);
		auto item_display_h = std::wstring{item_display};
		CoTaskMemFree(item_display);
		return winrt::to_string(item_display_h);
	}

	// ----------------

	constexpr auto hash_string(
		std::string_view const & string
	) const -> std::uint64_t {
		auto offset = std::uint64_t{14695981039346656037ull};
		auto prime = std::uint64_t{1099511628211ull};
		auto result = offset;
		for (auto & element : string) {
			result ^= static_cast<std::uint8_t>(element);
			result *= prime;
		}
		return result;
	}

	#pragma endregion

};
