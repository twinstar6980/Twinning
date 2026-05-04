module;

#include <gtk/gtk.h>
#include <flutter_linux/flutter_linux.h>
#include "./my_application.h"
#include "./common.hpp"

export module platform_integration_manager;

export {

	class PlatformIntegrationManager {

	private:

		std::add_pointer_t<FlMethodChannel> m_channel;

		std::add_pointer_t<GtkApplication> m_application;

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
			m_application{} {
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
		) -> gchar const * {
			static auto value = thiz.query_application_name();
			return static_cast<gchar const *>(value.data());
		}

		#pragma endregion

		#pragma region inject

		auto inject_MyApplication_activate(
			MyApplication &  host,
			GApplication * & with_application,
			FlView * &       also_view
		) -> void {
			thiz.m_application = GTK_APPLICATION(with_application);
			g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
			thiz.m_channel = fl_method_channel_new(
				fl_engine_get_binary_messenger(fl_view_get_engine(also_view)),
				std::format("{}/PlatformIntegrationManager", thiz.query_application_identifier()).data(),
				FL_METHOD_CODEC(codec)
			);
			fl_method_channel_set_method_call_handler(
				thiz.m_channel,
				[](
				FlMethodChannel * channel,
				FlMethodCall *    method_call,
				gpointer          user_data
			) -> void {
					auto & self = *reinterpret_cast<PlatformIntegrationManager *>(user_data);
					g_autoptr(FlMethodResponse) response = nullptr;
					self.handle(method_call, response);
					g_autoptr(GError) error = nullptr;
					if (!fl_method_call_respond(method_call, response, &error)) {
						g_warning("Failed to send response: %s", error->message);
					}
					return;
				},
				&thiz,
				nullptr
			);
			return;
		}

		auto inject_MyApplication_dispose(
			MyApplication & host,
			GObject * &     with_object
		) -> void {
			g_clear_object(&thiz.m_channel);
			return;
		}

		#pragma endregion

	private:

		#pragma region handle

		auto handle(
			FlMethodCall * const & call,
			FlMethodResponse * &   response
		) -> void {
			try {
				auto method = fl_method_call_get_name(call);
				auto raw_argument = fl_method_call_get_args(call);
				assert_test(fl_value_get_type(raw_argument) == FL_VALUE_TYPE_MAP);
				auto get_argument = [&](std::string_view const & name) -> FlValue * {
					return thiz.extract_flutter_value_map(raw_argument, name);
				};
				auto raw_result = fl_value_new_map();
				auto set_result = [&](std::string_view const & name, FlValue * const & value) -> void {
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
						[[maybe_unused]]
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
						[[maybe_unused]]
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
						[[maybe_unused]]
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
						[[maybe_unused]]
						auto detail = thiz.handle_push_system_notification(
							thiz.decode_flutter_value<std::string>(get_argument("title")),
							thiz.decode_flutter_value<std::string>(get_argument("description"))
						);
						break;
					}
					default: throw std::runtime_error{"Exception: invalid method"};
				}
				response = FL_METHOD_RESPONSE(fl_method_success_response_new(raw_result));
			}
			catch (...) {
				response = FL_METHOD_RESPONSE(fl_method_error_response_new("", thiz.parse_current_exception().data(), nullptr));
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
				state = false;
			}
			return std::make_tuple(state);
		}

		auto handle_update_application_extension(
			std::string const & name,
			bool const &        state
		) -> std::tuple<> {
			assert_test(name == "forwarder");
			if (name == "forwarder") {
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
				target = std::string{g_get_home_dir()};
			}
			if (type == "application_shared") {
				target = std::string{g_get_user_data_dir()} + "/" + thiz.query_application_identifier();
			}
			if (type == "application_temporary") {
				target = std::string{g_get_user_data_dir()} + "/" + thiz.query_application_identifier() + "/temporary";
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
			auto window = GTK_WINDOW(g_list_nth_data(gtk_application_get_windows(thiz.m_application), 0));
			auto dialog_action = GtkFileChooserAction{};
			if (type == "load_file") {
				dialog_action = GTK_FILE_CHOOSER_ACTION_OPEN;
			}
			if (type == "load_directory") {
				dialog_action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
			}
			if (type == "save_file") {
				dialog_action = GTK_FILE_CHOOSER_ACTION_SAVE;
			}
			g_autoptr(GtkFileChooserNative) dialog = gtk_file_chooser_native_new(
				"File Chooser",
				window,
				dialog_action,
				"_Accept",
				"_Cancel"
			);
			gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), multiply ? TRUE : FALSE);
			gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(dialog), TRUE);
			gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(dialog), TRUE);
			gtk_file_chooser_set_preview_widget_active(GTK_FILE_CHOOSER(dialog), FALSE);
			gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), location.data());
			if (type == "load_directory" || type == "save_file") {
				gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(dialog), TRUE);
			}
			if (type == "save_file") {
				gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
				gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), name.data());
			}
			auto target = std::vector<std::string>{};
			auto dialog_response = gtk_native_dialog_run(GTK_NATIVE_DIALOG(dialog));
			if (dialog_response == GTK_RESPONSE_ACCEPT) {
				g_autoptr(GSList) target_list = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dialog));
				for (auto target_item = target_list; target_item != nullptr; target_item = target_item->next) {
					target.emplace_back(static_cast<gchar *>(target_item->data));
				}
			}
			return std::make_tuple(target);
		}

		// ----------------

		auto handle_push_system_notification(
			std::string const & title,
			std::string const & description
		) -> std::tuple<> {
			// TODO
			return std::make_tuple();
		}

		#pragma endregion

		#pragma region invoke

		auto invoke(
			std::string const &                      method,
			std::map<std::string, FlValue *> const & argument
		) -> void {
			auto raw_argument = fl_value_new_map();
			for (auto & argument_item : argument) {
				thiz.infuse_flutter_value_map(raw_argument, argument_item.first, argument_item.second);
			}
			fl_method_channel_invoke_method(
				thiz.m_channel,
				method.data(),
				raw_argument,
				nullptr,
				nullptr,
				nullptr
			);
			return;
		}

		// ----------------

		auto invoke_receive_application_link(
			std::string const & target
		) -> void {
			return thiz.invoke(
				"receive_application_link",
				std::map<std::string, FlValue *>{{
					std::make_pair("target", thiz.encode_flutter_value(target)),
				}}
			);
		}

		#pragma endregion

		#pragma region utility

		template <typename TValue>
		auto encode_flutter_value(
			TValue const & ripe
		) const -> FlValue * {
			auto raw = std::add_pointer_t<FlValue>{nullptr};
			if constexpr (std::is_same_v<TValue, bool>) {
				raw = fl_value_new_bool(ripe);
			}
			if constexpr (std::is_same_v<TValue, std::string>) {
				raw = fl_value_new_string(ripe.data());
			}
			if constexpr (std::is_same_v<TValue, std::vector<std::string>>) {
				raw = fl_value_new_list();
				for (auto & ripe_item : ripe) {
					fl_value_append(raw, fl_value_new_string(ripe_item.data()));
				}
			}
			return raw;
		}

		template <typename TValue>
		auto decode_flutter_value(
			FlValue * const & raw
		) const -> TValue {
			auto ripe = TValue{};
			if constexpr (std::is_same_v<TValue, bool>) {
				assert_test(fl_value_get_type(raw) == FL_VALUE_TYPE_BOOL);
				ripe = fl_value_get_bool(raw);
			}
			if constexpr (std::is_same_v<TValue, std::string>) {
				assert_test(fl_value_get_type(raw) == FL_VALUE_TYPE_STRING);
				ripe = std::string{fl_value_get_string(raw)};
			}
			return ripe;
		}

		auto extract_flutter_value_map(
			FlValue * const &        map,
			std::string_view const & name
		) const -> FlValue * {
			assert_test(fl_value_get_type(map) == FL_VALUE_TYPE_MAP);
			auto value = fl_value_lookup_string(map, name.data());
			assert_test(value != nullptr);
			return value;
		}

		auto infuse_flutter_value_map(
			FlValue * &              map,
			std::string_view const & name,
			FlValue * const &        value
		) const -> void {
			assert_test(fl_value_get_type(map) == FL_VALUE_TYPE_MAP);
			fl_value_set_string_take(map, name.data(), value);
			return;
		}

		// ----------------

		auto query_application_identifier(
		) const -> std::string {
			return std::string{"com.twinstar.twinning.assistant"};
		}

		auto query_application_name(
		) const -> std::string {
			return std::string{"Twinning Assistant"};
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
			catch (...) {
				message = "UnknownException";
			}
			return message;
		}

		auto open_external_link(
			std::string const & link
		) const -> void {
			g_autoptr(GError) error = nullptr;
			auto state_b = gtk_show_uri_on_window(nullptr, link.data(), GDK_CURRENT_TIME, &error);
			if (!state_b) {
				throw std::runtime_error{std::string{"Exception: "} + error->message};
			}
			return;
		}

		#pragma endregion

	};

}
