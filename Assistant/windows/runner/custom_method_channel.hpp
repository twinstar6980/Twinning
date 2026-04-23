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
		flutter::MethodChannel<>{
			thiz.m_host->flutter_controller_->engine()->messenger(),
			std::format("{}.CustomMethodChannel", thiz.query_application_identifier()),
			&flutter::StandardMethodCodec::GetInstance(),
		}.SetMethodCallHandler(
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
			auto argument_map = std::get<flutter::EncodableMap>(*call.arguments());
			auto result_map = flutter::EncodableMap{};
			auto get_argument = [&]<typename TValue>(std::string_view const & name) -> TValue {
				auto item_f = argument_map.find(flutter::EncodableValue{name.data()});
				assert_test(item_f != argument_map.end());
				auto value = TValue{};
				if constexpr (std::is_same_v<TValue, bool>) {
					value = std::get<TValue>(item_f->second);
				}
				if constexpr (std::is_same_v<TValue, std::string>) {
					value = std::get<TValue>(item_f->second);
				}
				return value;
			};
			auto set_result = [&]<typename TValue>(std::string_view const & name, TValue const & value) -> void {
				auto value_f = flutter::EncodableValue{};
				if constexpr (std::is_same_v<TValue, std::string>) {
					value_f.emplace<std::string>(value.data());
				}
				if constexpr (std::is_same_v<TValue, std::vector<std::string>>) {
					value_f.emplace<flutter::EncodableList>();
					for (auto & value_item : value) {
						std::get<flutter::EncodableList>(value_f).emplace_back<std::string>(value_item.data());
					}
				}
				result_map.emplace(name.data(), value_f);
				return;
			};
			switch (hash_string(call.method_name())) {
				case hash_string("query_storage_item"): {
					auto detail = thiz.handle_query_storage_item(
						get_argument.operator ()<std::string>("type")
					);
					set_result("target", std::get<0>(detail));
					break;
				}
				case hash_string("reveal_storage_item"): {
					auto detail = thiz.handle_reveal_storage_item(
						get_argument.operator ()<std::string>("target")
					);
					break;
				}
				case hash_string("pick_storage_item"): {
					auto detail = thiz.handle_pick_storage_item(
						get_argument.operator ()<std::string>("type"),
						get_argument.operator ()<bool>("multiply"),
						get_argument.operator ()<std::string>("location"),
						get_argument.operator ()<std::string>("name")
					);
					set_result("target", std::get<0>(detail));
					break;
				}
				default: throw std::runtime_error{"Exception: invalid method"};
			}
			result->Success(result_map);
		}
		catch (...) {
			result->Error("", thiz.parse_current_exception());
		}
		return;
	}

	auto handle_query_storage_item(
		std::string const & type
	) -> std::tuple<std::string> {
		assert_test(type == "user_home" || type == "application_shared" || type == "application_cache");
		auto target = std::string{};
		if (type == "user_home") {
			target = thiz.query_known_folder_path(FOLDERID_Profile);
		}
		if (type == "application_shared") {
			target = thiz.query_known_folder_path(FOLDERID_RoamingAppData) + "\\" + thiz.query_application_identifier();
		}
		if (type == "application_cache") {
			target = thiz.query_known_folder_path(FOLDERID_RoamingAppData) + "\\" + thiz.query_application_identifier() + "\\cache";
		}
		return std::make_tuple(target);
	}

	auto handle_reveal_storage_item(
		std::string const & target
	) -> std::tuple<> {
		thiz.open_link(std::format("file://{}", target));
		return std::make_tuple();
	}

	auto handle_pick_storage_item(
		std::string const & type,
		bool const &        multiply,
		std::string const & location,
		std::string const & name
	) -> std::tuple<std::vector<std::string>> {
		assert_test(type == "load_file" || type == "load_directory" || type == "save_file");
		auto state_h = HRESULT{};
		auto location_h = winrt::to_hstring(location);
		auto name_h = winrt::to_hstring(name);
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
			if (multiply) {
				option |= FOS_ALLOWMULTISELECT;
			}
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
		auto target = std::vector<std::string>{};
		state_h = dialog->Show(thiz.m_host->GetHandle());
		if (state_h != HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
			winrt::check_hresult(state_h);
			if (type == "load_file" || type == "load_directory") {
				auto target_list = winrt::com_ptr<IShellItemArray>{};
				state_h = dialog.as<IFileOpenDialog>()->GetResults(target_list.put());
				winrt::check_hresult(state_h);
				auto target_count = DWORD{};
				state_h = target_list->GetCount(&target_count);
				winrt::check_hresult(state_h);
				for (auto target_index = DWORD{0}; target_index < target_count; ++target_index) {
					auto target_item = winrt::com_ptr<IShellItem>{};
					state_h = target_list->GetItemAt(target_index, target_item.put());
					winrt::check_hresult(state_h);
					target.emplace_back(thiz.resolve_shell_item_path(target_item));
				}
			}
			if (type == "save_file") {
				auto target_item = winrt::com_ptr<IShellItem>{};
				state_h = dialog->GetResult(target_item.put());
				winrt::check_hresult(state_h);
				target.emplace_back(thiz.resolve_shell_item_path(target_item));
			}
		}
		return std::make_tuple(target);
	}

	#pragma endregion

	#pragma region utility

	auto query_application_identifier(
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

	auto query_known_folder_path(
		KNOWNFOLDERID const & type
	) const -> std::string {
		auto state_h = HRESULT{};
		auto path_p = LPWSTR{};
		state_h = SHGetKnownFolderPath(type, 0, nullptr, &path_p);
		winrt::check_hresult(state_h);
		auto path_h = std::wstring{path_p};
		CoTaskMemFree(path_p);
		return winrt::to_string(path_h);
	}

	auto resolve_storage_long_path(
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

	auto resolve_shell_item_path(
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

	auto open_link(
		std::string const & link
	) const -> void {
		auto state_hi = HINSTANCE{};
		auto link_h = winrt::to_hstring(link);
		state_hi = ShellExecuteW(nullptr, L"open", link_h.data(), nullptr, nullptr, SW_SHOWNORMAL);
		assert_test(reinterpret_cast<INT_PTR>(state_hi) > 32);
		return;
	}

	#pragma endregion

};
