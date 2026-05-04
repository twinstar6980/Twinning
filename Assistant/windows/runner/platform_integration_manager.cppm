module;

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
#include "./utils.h"
#include "./win32_window.h"
#include "./flutter_window.h"
#include "./common.hpp"

export module platform_integration_manager;

export {

	class PlatformIntegrationManager {

	private:

		std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> m_channel;

		std::add_pointer_t<FlutterWindow> m_window;

	public:

		#pragma region construct

		~PlatformIntegrationManager(
		) = default;

		// ----------------

		PlatformIntegrationManager(
		) = delete;

		PlatformIntegrationManager(
			PlatformIntegrationManager const & that
		) = delete;

		PlatformIntegrationManager(
			PlatformIntegrationManager && that
		) = delete;

		#pragma endregion

		#pragma region operator

		auto operator =(
			PlatformIntegrationManager const & that
		) -> PlatformIntegrationManager & = delete;

		auto operator =(
			PlatformIntegrationManager && that
		) -> PlatformIntegrationManager & = delete;

		#pragma endregion

	private:

		#pragma region construct

		explicit PlatformIntegrationManager(
			std::nullptr_t _
		) :
			m_channel{},
			m_window{} {
			return;
		}

		#pragma endregion

		#pragma region utility

		static constexpr auto hash_string(
			std::string_view const & string
		) -> std::uint64_t {
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

	public:

		#pragma region singleton

		static auto instance(
		) -> PlatformIntegrationManager & {
			static auto value = PlatformIntegrationManager{nullptr};
			return value;
		}

		#pragma endregion

		#pragma region exposed

		auto exposed_application_name(
		) -> std::wstring const & {
			static auto value = std::wstring{winrt::to_hstring(thiz.query_application_name())};
			return value;
		}

		#pragma endregion

		#pragma region inject

		// ReSharper disable once CppInconsistentNaming
		auto inject_wWinMain(
			std::nullptr_t host,
			HINSTANCE &    with_instance,
			HINSTANCE &    with_prev,
			wchar_t * &    with_command_line,
			int &          with_show_command,
			bool &         also_exit
		) -> void {
			also_exit = false;
			auto argument = GetCommandLineArguments();
			if (argument.size() == 1 && std::regex_search(argument.front(), std::regex{R"(^([a-z][a-z0-9\+\-\.]+):)", std::regex_constants::icase})) {
				auto window = ::FindWindowW(L"FLUTTER_RUNNER_WIN32_WINDOW", thiz.exposed_application_name().data());
				if (window != nullptr) {
					auto data = COPYDATASTRUCT{};
					data.dwData = WM_USER + 1;
					data.cbData = static_cast<DWORD>(argument.front().size() * sizeof(std::string::value_type));
					data.lpData = argument.front().data();
					SendMessageW(window, WM_COPYDATA, reinterpret_cast<WPARAM>(window), reinterpret_cast<LPARAM>(&data));
					also_exit = true;
					return;
				}
			}
			SetProcessPreferredUILanguages(MUI_LANGUAGE_NAME, L"en-US\0\0", nullptr);
			std::locale::global(std::locale::classic());
			return;
		}

		// ReSharper disable once CppInconsistentNaming
		auto inject_Win32Window_MessageHandler(
			Win32Window &  host,
			HWND &         with_window,
			UINT const &   with_message,
			WPARAM const & with_wparam,
			LPARAM const & with_lparam
		) -> void {
			if (with_message == WM_COPYDATA) {
				auto & data = *reinterpret_cast<COPYDATASTRUCT *>(with_lparam);
				if (data.dwData == WM_USER + 1) {
					auto link = std::string{static_cast<std::string::value_type *>(data.lpData), static_cast<std::size_t>(data.cbData)};
					thiz.bring_window_to_foreground(with_window);
					thiz.invoke_receive_application_link(link);
				}
			}
			return;
		}

		// ReSharper disable once CppInconsistentNaming
		auto inject_FlutterWindow_OnCreate(
			FlutterWindow &                                   host,
			std::unique_ptr<flutter::FlutterViewController> & also_flutter_controller
		) -> void {
			thiz.m_window = &host;
			thiz.m_channel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
				also_flutter_controller->engine()->messenger(),
				std::format("{}/PlatformIntegrationManager", thiz.query_application_identifier()),
				&flutter::StandardMethodCodec::GetInstance()
			);
			thiz.m_channel->SetMethodCallHandler(
				[&](
				flutter::MethodCall<flutter::EncodableValue> const &            call,
				std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result
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
			flutter::MethodCall<flutter::EncodableValue> const &              call,
			std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> & result
		) -> void {
			try {
				auto method = std::string_view{call.method_name()};
				auto raw_argument = std::get<flutter::EncodableMap>(*call.arguments());
				auto get_argument = [&](std::string_view const & name) -> flutter::EncodableValue const & {
					return thiz.extract_flutter_value_map(raw_argument, name);
				};
				auto raw_result = flutter::EncodableMap{};
				auto set_result = [&](std::string_view const & name, flutter::EncodableValue const & value) -> void {
					return thiz.infuse_flutter_value_map(raw_result, name, value);
				};
				switch (hash_string(method)) {
					case hash_string("check_application_permission"): {
						auto detail = thiz.handle_check_application_permission(
							thiz.decode_flutter_value<std::string>(get_argument("name"))
						);
						set_result("state", thiz.encode_flutter_value(std::get<0>(detail)));
						break;
					}
					case hash_string("update_application_permission"): {
						auto detail = thiz.handle_update_application_permission(
							thiz.decode_flutter_value<std::string>(get_argument("name"))
						);
						break;
					}
					case hash_string("check_application_extension"): {
						auto detail = thiz.handle_check_application_extension(
							thiz.decode_flutter_value<std::string>(get_argument("name"))
						);
						set_result("state", thiz.encode_flutter_value(std::get<0>(detail)));
						break;
					}
					case hash_string("update_application_extension"): {
						auto detail = thiz.handle_update_application_extension(
							thiz.decode_flutter_value<std::string>(get_argument("name")),
							thiz.decode_flutter_value<bool>(get_argument("state"))
						);
						break;
					}
					case hash_string("query_storage_item"): {
						auto detail = thiz.handle_query_storage_item(
							thiz.decode_flutter_value<std::string>(get_argument("type"))
						);
						set_result("target", thiz.encode_flutter_value(std::get<0>(detail)));
						break;
					}
					case hash_string("reveal_storage_item"): {
						auto detail = thiz.handle_reveal_storage_item(
							thiz.decode_flutter_value<std::string>(get_argument("target"))
						);
						break;
					}
					case hash_string("pick_storage_item"): {
						auto detail = thiz.handle_pick_storage_item(
							thiz.decode_flutter_value<std::string>(get_argument("type")),
							thiz.decode_flutter_value<bool>(get_argument("multiply")),
							thiz.decode_flutter_value<std::string>(get_argument("location")),
							thiz.decode_flutter_value<std::string>(get_argument("name"))
						);
						set_result("target", thiz.encode_flutter_value(std::get<0>(detail)));
						break;
					}
					case hash_string("push_system_notification"): {
						auto detail = thiz.handle_push_system_notification(
							thiz.decode_flutter_value<std::string>(get_argument("title")),
							thiz.decode_flutter_value<std::string>(get_argument("description"))
						);
						break;
					}
					case hash_string("on_windows_query_storage_long_path"): {
						auto detail = thiz.handle_on_windows_query_storage_long_path(
							thiz.decode_flutter_value<std::string>(get_argument("source"))
						);
						set_result("destination", thiz.encode_flutter_value(std::get<0>(detail)));
						break;
					}
					default: throw std::runtime_error{"Exception: invalid method"};
				}
				result->Success(raw_result);
			}
			catch (...) {
				result->Error("", thiz.parse_current_exception());
			}
			return;
		}

		// ----------------

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
				thiz.open_external_link(std::string{"ms-settings:notifications"});
			}
			return std::make_tuple();
		}

		// ----------------

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

		// ----------------

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
			thiz.open_external_link(std::format("file://{}", target));
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
			state_h = dialog->Show(thiz.m_window->GetHandle());
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
						auto target_item_path = thiz.resolve_shell_item_path(target_item);
						auto target_item_path_long = thiz.query_storage_long_path(target_item_path);
						target.emplace_back(target_item_path_long);
					}
				}
				if (type == "save_file") {
					auto target_item = winrt::com_ptr<IShellItem>{};
					state_h = dialog->GetResult(target_item.put());
					winrt::check_hresult(state_h);
					auto target_item_path = thiz.resolve_shell_item_path(target_item);
					auto target_item_parent_end = target_item_path.find_last_of("\\");
					assert_test(target_item_parent_end != std::string::npos);
					auto target_item_parent = target_item_path.substr(0, target_item_parent_end);
					auto target_item_name = target_item_path.substr(target_item_parent_end + 1);
					auto target_item_path_long = thiz.query_storage_long_path(target_item_parent) + "\\" + target_item_name;
					target.emplace_back(target_item_path_long);
				}
			}
			return std::make_tuple(target);
		}

		// ----------------

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
					thiz.bring_window_to_foreground(thiz.m_window->GetHandle());
					return;
				}
			);
			auto notifier = winrt::Windows::UI::Notifications::ToastNotificationManager::CreateToastNotifier();
			notifier.Show(notification);
			return std::make_tuple();
		}

		// ----------------

		auto handle_on_windows_query_storage_long_path(
			std::string const & source
		) -> std::tuple<std::string> {
			auto destination = thiz.query_storage_long_path(source);
			return std::make_tuple(destination);
		}

		#pragma endregion

		#pragma region invoke

		auto invoke(
			std::string const &                                    method,
			std::map<std::string, flutter::EncodableValue> const & argument
		) -> void {
			auto raw_argument = std::make_unique<flutter::EncodableValue>();
			raw_argument->emplace<flutter::EncodableMap>();
			for (auto & argument_item : argument) {
				thiz.infuse_flutter_value_map(std::get<flutter::EncodableMap>(*raw_argument), argument_item.first, argument_item.second);
			}
			thiz.m_channel->InvokeMethod(method, std::move(raw_argument), nullptr);
			return;
		}

		// ----------------

		auto invoke_receive_application_link(
			std::string const & target
		) -> void {
			return thiz.invoke(
				"receive_application_link",
				std::map<std::string, flutter::EncodableValue>{{
					std::make_pair("target", thiz.encode_flutter_value(target)),
				}}
			);
		}

		#pragma endregion

		#pragma region utility

		template <typename TValue>
		auto encode_flutter_value(
			TValue const & ripe
		) const -> flutter::EncodableValue {
			auto raw = flutter::EncodableValue{};
			if constexpr (std::is_same_v<TValue, bool>) {
				raw.emplace<bool>(ripe);
			}
			if constexpr (std::is_same_v<TValue, std::string>) {
				raw.emplace<std::string>(ripe.data());
			}
			if constexpr (std::is_same_v<TValue, std::vector<std::string>>) {
				raw.emplace<flutter::EncodableList>();
				for (auto & ripe_item : ripe) {
					std::get<flutter::EncodableList>(raw).emplace_back<std::string>(ripe_item.data());
				}
			}
			return raw;
		}

		template <typename TValue>
		auto decode_flutter_value(
			flutter::EncodableValue const & raw
		) const -> TValue {
			auto ripe = TValue{};
			if constexpr (std::is_same_v<TValue, bool>) {
				ripe = std::get<TValue>(raw);
			}
			if constexpr (std::is_same_v<TValue, std::string>) {
				ripe = std::get<TValue>(raw);
			}
			return ripe;
		}

		auto extract_flutter_value_map(
			flutter::EncodableMap const & map,
			std::string_view const &      name
		) const -> flutter::EncodableValue const & {
			auto raw_name = flutter::EncodableValue{std::in_place_type_t<std::string>{}, name.data()};
			return map.at(raw_name);
		}

		auto infuse_flutter_value_map(
			flutter::EncodableMap &         map,
			std::string_view const &        name,
			flutter::EncodableValue const & value
		) const -> void {
			auto raw_name = flutter::EncodableValue{std::in_place_type_t<std::string>{}, name.data()};
			map.emplace(std::move(raw_name), value);
			return;
		}

		// ----------------

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

		auto query_application_name(
		) const -> std::string {
			if (!thiz.check_application_packaged()) {
				return std::string{"Twinning Assistant"};
			}
			return winrt::to_string(winrt::Windows::ApplicationModel::Package::Current().DisplayName());
		}

		// ----------------

		auto parse_current_exception(
		) const -> std::string {
			auto message = std::string{};
			try {
				std::rethrow_exception(std::current_exception());
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

		auto open_external_link(
			std::string const & link
		) const -> void {
			auto state_hi = HINSTANCE{};
			auto link_h = winrt::to_hstring(link);
			state_hi = ShellExecuteW(nullptr, L"open", link_h.data(), nullptr, nullptr, SW_SHOWNORMAL);
			assert_test(reinterpret_cast<INT_PTR>(state_hi) > 32);
			return;
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

		auto query_storage_long_path(
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

		auto bring_window_to_foreground(
			HWND const & window
		) const -> void {
			if (IsIconic(window)) {
				ShowWindow(window, SW_RESTORE);
			}
			keybd_event(VK_MENU, 0, 0, 0);
			keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
			SetForegroundWindow(window);
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

}
