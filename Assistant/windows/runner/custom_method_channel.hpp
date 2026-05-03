#pragma once

#include <ShlObj_core.h>
#include <Windows.h>
#include <appmodel.h>
#include <NotificationActivationCallback.h>
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.UI.Notifications.h>
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
				thiz.handle(call, result);
				return;
			}
		);
		thiz.register_notification_activation_callback();
		return;
	}

	#pragma endregion

private:

	#pragma region handle

	auto handle(
		flutter::MethodCall<> const &              call,
		std::unique_ptr<flutter::MethodResult<>> & result
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
				if constexpr (std::is_same_v<TValue, bool>) {
					value_f.emplace<bool>(value);
				}
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
				case hash_string("check_application_permission"): {
					auto detail = thiz.handle_check_application_permission(
						get_argument.operator ()<std::string>("name")
					);
					set_result("state", std::get<0>(detail));
					break;
				}
				case hash_string("update_application_permission"): {
					auto detail = thiz.handle_update_application_permission(
						get_argument.operator ()<std::string>("name")
					);
					break;
				}
				case hash_string("check_application_extension"): {
					auto detail = thiz.handle_check_application_extension(
						get_argument.operator ()<std::string>("name")
					);
					set_result("state", std::get<0>(detail));
					break;
				}
				case hash_string("update_application_extension"): {
					auto detail = thiz.handle_update_application_extension(
						get_argument.operator ()<std::string>("name"),
						get_argument.operator ()<bool>("state")
					);
					break;
				}
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
				case hash_string("push_system_notification"): {
					auto detail = thiz.handle_push_system_notification(
						get_argument.operator ()<std::string>("title"),
						get_argument.operator ()<std::string>("description")
					);
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

	auto handle_check_application_permission(
		std::string const & name
	) -> std::tuple<bool> {
		assert_test(name == "storage" || name == "notification");
		auto state = false;
		if (name == "storage") {
			state = true;
		}
		if (name == "notification") {
			state = true;
		}
		return std::make_tuple(state);
	}

	auto handle_update_application_permission(
		std::string const & name
	) -> std::tuple<> {
		assert_test(name == "storage" || name == "notification");
		if (name == "storage") {
		}
		if (name == "notification") {
			thiz.open_link(std::string{"ms-settings:notifications"});
		}
		return std::make_tuple();
	}

	auto handle_check_application_extension(
		std::string const & name
	) -> std::tuple<bool> {
		assert_test(name == "forwarder");
		auto state = false;
		if (name == "forwarder") {
			auto state_path_string = std::format("{}\\{}\\extension\\forwarder", thiz.query_known_folder_path(FOLDERID_RoamingAppData), thiz.query_application_identifier());
			auto state_path = std::filesystem::path{reinterpret_cast<std::u8string const &>(state_path_string)};
			auto state_exist = std::filesystem::exists(state_path);
			state = state_exist;
		}
		return std::make_tuple(state);
	}

	auto handle_update_application_extension(
		std::string const & name,
		bool const &        state
	) -> std::tuple<> {
		assert_test(name == "forwarder");
		if (name == "forwarder") {
			auto state_path_string = std::format("{}\\{}\\extension\\forwarder", thiz.query_known_folder_path(FOLDERID_RoamingAppData), thiz.query_application_identifier());
			auto state_path = std::filesystem::path{reinterpret_cast<std::u8string const &>(state_path_string)};
			auto state_exist = std::filesystem::exists(state_path);
			if (!state && state_exist) {
				std::filesystem::remove_all(state_path);
			}
			if (state && !state_exist) {
				std::filesystem::create_directories(state_path);
			}
		}
		return std::make_tuple();
	}

	auto handle_query_storage_item(
		std::string const & type
	) -> std::tuple<std::string> {
		assert_test(type == "user_home" || type == "application_shared" || type == "application_temporary");
		auto target = std::string{};
		if (type == "user_home") {
			target = thiz.query_known_folder_path(FOLDERID_Profile);
		}
		if (type == "application_shared") {
			target = thiz.query_known_folder_path(FOLDERID_RoamingAppData) + "\\" + thiz.query_application_identifier();
		}
		if (type == "application_temporary") {
			target = thiz.query_known_folder_path(FOLDERID_RoamingAppData) + "\\" + thiz.query_application_identifier() + "\\temporary";
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

	auto handle_push_system_notification(
		std::string const & title,
		std::string const & description
	) -> std::tuple<> {
		if (!thiz.check_application_packaged()) {
			return std::make_tuple();
		}
		auto toast = winrt::Windows::Data::Xml::Dom::XmlDocument{};
		auto xml_toast = toast.CreateElement(L"toast");
		auto xml_visual = toast.CreateElement(L"visual");
		auto xml_binding = toast.CreateElement(L"binding");
		xml_binding.SetAttribute(L"template", L"ToastGeneric");
		auto xml_text_title = toast.CreateElement(L"text");
		xml_text_title.AppendChild(toast.CreateTextNode(winrt::to_hstring(title)));
		xml_binding.AppendChild(xml_text_title);
		auto xml_text_description = toast.CreateElement(L"text");
		xml_text_description.AppendChild(toast.CreateTextNode(winrt::to_hstring(description)));
		xml_binding.AppendChild(xml_text_description);
		xml_visual.AppendChild(xml_binding);
		xml_toast.AppendChild(xml_visual);
		toast.AppendChild(xml_toast);
		auto notification = winrt::Windows::UI::Notifications::ToastNotification{toast};
		notification.ExpiresOnReboot(false);
		notification.Activated(
			[&](
			winrt::Windows::UI::Notifications::ToastNotification const & sender,
			winrt::Windows::Foundation::IInspectable const &             args
		) -> auto {
				auto window = thiz.m_host->GetHandle();
				if (IsIconic(window)) {
					ShowWindow(window, SW_RESTORE);
				}
				keybd_event(VK_MENU, 0, 0, 0);
				keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
				SetForegroundWindow(window);
				return;
			}
		);
		auto notifier = winrt::Windows::UI::Notifications::ToastNotificationManager::CreateToastNotifier();
		notifier.Show(notification);
		return std::make_tuple();
	}

	#pragma endregion

	#pragma region utility

	auto check_application_packaged(
	) const -> bool {
		auto length = UINT32{};
		auto state_l = GetCurrentPackageFullName(&length, nullptr);
		return state_l != APPMODEL_ERROR_NO_PACKAGE;
	}

	auto query_application_identifier(
	) const -> std::string {
		if (!thiz.check_application_packaged()) {
			return std::string{"com.twinstar.twinning.assistant"};
		}
		return winrt::to_string(winrt::Windows::ApplicationModel::Package::Current().Id().Name());
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

	// ----------------

	auto register_notification_activation_callback(
	) const -> void {
		if (!thiz.check_application_packaged()) {
			return;
		}
		auto state_h = HRESULT{};
		auto rclsid = winrt::guid{"3FCD5C89-78F3-489B-88E5-37CBC3C3FC1A"};
		auto factory = winrt::make_self<NotificationActivationCallbackFactory>();
		auto registration = DWORD{};
		state_h = CoRegisterClassObject(rclsid, factory.get(), CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &registration);
		winrt::check_hresult(state_h);
		return;
	}

	class NotificationActivationCallback :
		public winrt::implements<NotificationActivationCallback, INotificationActivationCallback> {

	public:

		#pragma region constructor

		explicit NotificationActivationCallback(
		) :
			winrt::implements<NotificationActivationCallback, INotificationActivationCallback>{} {
			return;
		}

		#pragma endregion

		#pragma region implement INotificationActivationCallback

		// ReSharper disable CppInconsistentNaming CppEnforceFunctionDeclarationStyle

		virtual IFACEMETHODIMP Activate(
			LPCWSTR                              appUserModelId,
			LPCWSTR                              invokedArgs,
			NOTIFICATION_USER_INPUT_DATA const * data,
			ULONG                                count
		) override {
			return S_OK;
		}

		// ReSharper restore CppInconsistentNaming CppEnforceFunctionDeclarationStyle

		#pragma endregion

	};

	class NotificationActivationCallbackFactory :
		public winrt::implements<NotificationActivationCallbackFactory, IClassFactory> {

	public:

		#pragma region constructor

		explicit NotificationActivationCallbackFactory(
		) :
			winrt::implements<NotificationActivationCallbackFactory, IClassFactory>{} {
			return;
		}

		#pragma endregion

		#pragma region implement IClassFactory

		// ReSharper disable CppInconsistentNaming CppEnforceFunctionDeclarationStyle

		virtual IFACEMETHODIMP CreateInstance(
			IUnknown * pUnkOuter,
			REFIID     riid,
			void * *   ppvObject
		) override {
			try {
				return winrt::make<NotificationActivationCallback>()->QueryInterface(riid, ppvObject);
			}
			catch (...) {
				return winrt::to_hresult();
			}
		}

		virtual IFACEMETHODIMP LockServer(
			BOOL fLock
		) override {
			return S_OK;
		}

		// ReSharper restore CppInconsistentNaming CppEnforceFunctionDeclarationStyle

		#pragma endregion

	};

	#pragma endregion

};
