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

		bool m_drag_inside;

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
			m_application{},
			m_drag_inside{false} {
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
					auto & self = *static_cast<PlatformIntegrationManager *>(user_data);
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
			thiz.register_notification_support();
			thiz.register_drag_drop_support();
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
				auto set_result = [&](std::string_view const & name, FlValue * && value) -> void {
					return thiz.infuse_flutter_value_map(raw_result, name, std::move(value));
				};
				switch (hash_string(method)) {
					case hash_string("check_application_permission"): {
						auto detail = thiz.handle_check_application_permission(
							thiz.decode_flutter_value<std::string>(get_argument("name"))
						);
						set_result("state", thiz.encode_flutter_value(std::move(std::get<0>(detail))));
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
						set_result("state", thiz.encode_flutter_value(std::move(std::get<0>(detail))));
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
						set_result("target", thiz.encode_flutter_value(std::move(std::get<0>(detail))));
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
						set_result("target", thiz.encode_flutter_value(std::move(std::get<0>(detail))));
						break;
					}
					case hash_string("query_system_theme"): {
						auto detail = thiz.handle_query_system_theme(
						);
						set_result("accent", thiz.encode_flutter_value(std::move(std::get<0>(detail))));
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
					case hash_string("query_screen_placement"): {
						auto detail = thiz.handle_query_screen_placement(
						);
						set_result("x", thiz.encode_flutter_value(std::move(std::get<0>(detail))));
						set_result("y", thiz.encode_flutter_value(std::move(std::get<1>(detail))));
						set_result("width", thiz.encode_flutter_value(std::move(std::get<2>(detail))));
						set_result("height", thiz.encode_flutter_value(std::move(std::get<3>(detail))));
						break;
					}
					case hash_string("query_window_placement"): {
						auto detail = thiz.handle_query_window_placement(
						);
						set_result("x", thiz.encode_flutter_value(std::move(std::get<0>(detail))));
						set_result("y", thiz.encode_flutter_value(std::move(std::get<1>(detail))));
						set_result("width", thiz.encode_flutter_value(std::move(std::get<2>(detail))));
						set_result("height", thiz.encode_flutter_value(std::move(std::get<3>(detail))));
						break;
					}
					case hash_string("update_window_placement"): {
						[[maybe_unused]]
						auto detail = thiz.handle_update_window_placement(
							thiz.decode_flutter_value<std::int64_t>(get_argument("x")),
							thiz.decode_flutter_value<std::int64_t>(get_argument("y")),
							thiz.decode_flutter_value<std::int64_t>(get_argument("width")),
							thiz.decode_flutter_value<std::int64_t>(get_argument("height"))
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
			return std::make_tuple(std::move(state));
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
			return std::make_tuple(std::move(state));
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
			assert_test(type == "user_home" || type == "application_shared" || type == "application_persistent" || type == "application_temporary" || type == "application_cache");
			auto target = std::string{};
			if (type == "user_home") {
				target = std::string{g_get_home_dir()};
			}
			if (type == "application_shared") {
				target = std::string{g_get_user_data_dir()} + "/" + thiz.query_application_identifier();
			}
			if (type == "application_persistent") {
				target = std::string{g_get_user_data_dir()} + "/" + thiz.query_application_identifier() + "/persistent";
			}
			if (type == "application_temporary") {
				target = std::string{g_get_user_data_dir()} + "/" + thiz.query_application_identifier() + "/temporary";
			}
			if (type == "application_cache") {
				target = std::string{g_get_user_data_dir()} + "/" + thiz.query_application_identifier() + "/cache";
			}
			return std::make_tuple(std::move(target));
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
			auto window = thiz.get_current_window();
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
			return std::make_tuple(std::move(target));
		}

		// ----------------

		auto handle_query_system_theme(
		) -> std::tuple<std::optional<std::int64_t>> {
			auto state_b = gboolean{};
			auto window = thiz.get_current_window();
			auto context = gtk_widget_get_style_context(GTK_WIDGET(window));
			auto accent_color = GdkRGBA{};
			state_b = gtk_style_context_lookup_color(context, "theme_selected_bg_color", &accent_color);
			auto accent = std::optional<std::int64_t>{};
			if (state_b) {
				accent.emplace(
					static_cast<std::int64_t>(
						(std::llround(accent_color.alpha * 255.0) << 24) |
						(std::llround(accent_color.red * 255.0) << 16) |
						(std::llround(accent_color.green * 255.0) << 8) |
						(std::llround(accent_color.blue * 255.0) << 0)
					)
				);
			}
			return std::make_tuple(std::move(accent));
		}

		// ----------------

		auto handle_push_system_notification(
			std::string const & title,
			std::string const & description
		) -> std::tuple<> {
			g_autoptr(GDBusConnection) connection = nullptr;
			g_autoptr(GError) error = nullptr;
			connection = g_bus_get_sync(G_BUS_TYPE_SESSION, nullptr, &error);
			if (connection == nullptr) {
				throw std::runtime_error{std::string{"Exception: "} + error->message};
			}
			auto notification = g_variant_new(
				"(susssasa{sv}i)",
				thiz.exposed_application_name(),
				0,
				"",
				title.data(),
				description.data(),
				nullptr,
				nullptr,
				-1
			);
			g_dbus_connection_call(
				connection,
				"org.freedesktop.Notifications",
				"/org/freedesktop/Notifications",
				"org.freedesktop.Notifications",
				"Notify",
				notification,
				nullptr,
				G_DBUS_CALL_FLAGS_NONE,
				-1,
				nullptr,
				nullptr,
				nullptr
			);
			return std::make_tuple();
		}

		// ----------------

		auto handle_query_screen_placement(
		) -> std::tuple<std::int64_t, std::int64_t, std::int64_t, std::int64_t> {
			auto monitor = thiz.get_current_monitor();
			auto rect = GdkRectangle{};
			gdk_monitor_get_workarea(monitor, &rect);
			auto x = static_cast<std::int64_t>(rect.x);
			auto y = static_cast<std::int64_t>(rect.y);
			auto width = static_cast<std::int64_t>(rect.width);
			auto height = static_cast<std::int64_t>(rect.height);
			return std::make_tuple(std::move(x), std::move(y), std::move(width), std::move(height));
		}

		auto handle_query_window_placement(
		) -> std::tuple<std::int64_t, std::int64_t, std::int64_t, std::int64_t> {
			auto window = thiz.get_current_window();
			auto rect = GdkRectangle{};
			gtk_window_get_position(window, &rect.x, &rect.y);
			gtk_window_get_size(window, &rect.width, &rect.height);
			auto x = static_cast<std::int64_t>(rect.x);
			auto y = static_cast<std::int64_t>(rect.y);
			auto width = static_cast<std::int64_t>(rect.width);
			auto height = static_cast<std::int64_t>(rect.height);
			return std::make_tuple(std::move(x), std::move(y), std::move(width), std::move(height));
		}

		auto handle_update_window_placement(
			std::int64_t const & x,
			std::int64_t const & y,
			std::int64_t const & width,
			std::int64_t const & height
		) -> std::tuple<> {
			auto window = thiz.get_current_window();
			auto actual_x = static_cast<gint>(x);
			auto actual_y = static_cast<gint>(y);
			auto actual_width = static_cast<gint>(width);
			auto actual_height = static_cast<gint>(height);
			gtk_window_move(window, actual_x, actual_y);
			gtk_window_resize(window, actual_width, actual_height);
			return std::make_tuple();
		}

		#pragma endregion

		#pragma region invoke

		auto invoke(
			std::string const &                 method,
			std::map<std::string, FlValue *> && argument
		) -> void {
			auto raw_argument = fl_value_new_map();
			for (auto & argument_item : argument) {
				thiz.infuse_flutter_value_map(raw_argument, argument_item.first, std::move(argument_item.second));
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
					std::make_pair("target", thiz.encode_flutter_value(auto{target})),
				}}
			);
		}

		// ----------------

		auto invoke_receive_application_drag_enter(
		) -> void {
			return thiz.invoke(
				"receive_application_drag_enter",
				std::map<std::string, FlValue *>{
				}
			);
		}

		auto invoke_receive_application_drag_over(
			std::int64_t const & location_x,
			std::int64_t const & location_y
		) -> void {
			return thiz.invoke(
				"receive_application_drag_over",
				std::map<std::string, FlValue *>{{
					std::make_pair("location_x", thiz.encode_flutter_value(auto{location_x})),
					std::make_pair("location_y", thiz.encode_flutter_value(auto{location_y})),
				}}
			);
		}

		auto invoke_receive_application_drag_leave(
		) -> void {
			return thiz.invoke(
				"receive_application_drag_leave",
				std::map<std::string, FlValue *>{
				}
			);
		}

		auto invoke_receive_application_drag_drop(
			std::vector<std::string> const & target
		) -> void {
			return thiz.invoke(
				"receive_application_drag_drop",
				std::map<std::string, FlValue *>{{
					std::make_pair("target", thiz.encode_flutter_value(auto{target})),
				}}
			);
		}

		#pragma endregion

		#pragma region support

		auto register_notification_support(
		) -> void {
			return;
		}

		// ----------------

		auto register_drag_drop_support(
		) -> void {
			auto window = thiz.get_current_window();
			auto widget = GTK_WIDGET(window);
			auto target = GtkTargetEntry{
				.target = const_cast<gchar *>("text/uri-list"),
				.flags = GTK_TARGET_OTHER_APP,
				.info = 1,
			};
			gtk_drag_dest_set(
				widget,
				GTK_DEST_DEFAULT_ALL,
				&target,
				1,
				static_cast<GdkDragAction>(GDK_ACTION_MOVE | GDK_ACTION_COPY | GDK_ACTION_LINK)
			);
			g_signal_connect(
				widget,
				"drag-motion",
				G_CALLBACK(
					+[](
					GtkWidget *      widget,
					GdkDragContext * context,
					gint             x,
					gint             y,
					guint            time,
					gpointer         user_data
				) -> gboolean {
						auto & self = *static_cast<PlatformIntegrationManager *>(user_data);
						if (!self.m_drag_inside) {
							self.m_drag_inside = true;
							self.invoke_receive_application_drag_enter();
						}
						else {
							auto location_x = static_cast<std::int64_t>(x);
							auto location_y = static_cast<std::int64_t>(y);
							self.invoke_receive_application_drag_over(location_x, location_y);
						}
						return TRUE;
					}
				),
				&thiz
			);
			g_signal_connect(
				widget,
				"drag-leave",
				G_CALLBACK(
					+[](
					GtkWidget *      widget,
					GdkDragContext * context,
					guint            time,
					gpointer         user_data
				) -> void {
						auto & self = *static_cast<PlatformIntegrationManager *>(user_data);
						self.m_drag_inside = false;
						g_autoptr(GdkEvent) event = gtk_get_current_event();
						assert_test(event != nullptr);
						if (event->type != GDK_DROP_START) {
							self.invoke_receive_application_drag_leave();
						}
						return;
					}
				),
				&thiz
			);
			g_signal_connect(
				widget,
				"drag-data-received",
				G_CALLBACK(
					+[](
					GtkWidget *        widget,
					GdkDragContext *   context,
					gint               x,
					gint               y,
					GtkSelectionData * data,
					guint              info,
					guint              time,
					gpointer           user_data
				) -> void {
						auto & self = *static_cast<PlatformIntegrationManager *>(user_data);
						self.m_drag_inside = false;
						g_auto(GStrv) target_uri = gtk_selection_data_get_uris(data);
						assert_test(target_uri != nullptr);
						auto has_invalid_item = false;
						auto target = std::vector<std::string>{};
						for (auto target_index = std::size_t{0}; target_uri[target_index] != nullptr; ++target_index) {
							g_autofree gchar * target_item = g_filename_from_uri(target_uri[target_index], nullptr, nullptr);
							if (target_item == nullptr) {
								has_invalid_item = true;
								break;
							}
							target.emplace_back(target_item);
						}
						if (!has_invalid_item) {
							self.invoke_receive_application_drag_drop(target);
						}
						gtk_drag_finish(context, has_invalid_item ? FALSE : TRUE, FALSE, time);
						return;
					}
				),
				&thiz
			);
			return;
		}

		#pragma endregion

		#pragma region utility

		template <typename TFinalizer>
		auto make_finalizer(
			TFinalizer const & finalizer
		) -> auto {
			auto finalizer_wrapper = [&](auto it) {
				delete it;
				finalizer();
			};
			return std::unique_ptr<std::uint8_t, decltype(finalizer_wrapper)>{new std::uint8_t{}, std::move(finalizer_wrapper)};
		}

		// ----------------

		template <typename TValue>
		auto encode_flutter_value(
			TValue && ripe
		) const -> FlValue * {
			auto raw = std::add_pointer_t<FlValue>{nullptr};
			if constexpr (std::is_same_v<TValue, bool>) {
				raw = fl_value_new_bool(ripe);
			}
			else if constexpr (std::is_same_v<TValue, std::int64_t>) {
				raw = fl_value_new_int(ripe);
			}
			else if constexpr (std::is_same_v<TValue, std::optional<std::int64_t>>) {
				if (!ripe.has_value()) {
					raw = fl_value_new_null();
				}
				else {
					raw = fl_value_new_int(ripe.value());
				}
			}
			else if constexpr (std::is_same_v<TValue, std::string>) {
				raw = fl_value_new_string_sized(ripe.data(), ripe.size());
			}
			else if constexpr (std::is_same_v<TValue, std::vector<std::string>>) {
				raw = fl_value_new_list();
				for (auto & ripe_item : ripe) {
					fl_value_append(raw, fl_value_new_string_sized(ripe_item.data(), ripe_item.size()));
				}
			}
			else {
				static_assert(false);
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
			else if constexpr (std::is_same_v<TValue, std::int64_t>) {
				assert_test(fl_value_get_type(raw) == FL_VALUE_TYPE_INT);
				ripe = fl_value_get_int(raw);
			}
			else if constexpr (std::is_same_v<TValue, std::string>) {
				assert_test(fl_value_get_type(raw) == FL_VALUE_TYPE_STRING);
				ripe = std::string{fl_value_get_string(raw)};
			}
			else {
				static_assert(false);
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
			FlValue * &&             value
		) const -> void {
			assert_test(value != nullptr);
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
			auto state_b = gboolean{};
			g_autoptr(GError) error = nullptr;
			state_b = gtk_show_uri_on_window(nullptr, link.data(), GDK_CURRENT_TIME, &error);
			if (!state_b) {
				throw std::runtime_error{std::string{"Exception: "} + error->message};
			}
			return;
		}

		auto get_current_monitor(
		) const -> GdkMonitor * {
			auto display = gdk_display_get_default();
			assert_test(display != nullptr);
			auto monitor = gdk_display_get_primary_monitor(display);
			if (monitor == nullptr) {
				auto monitor_count = gdk_display_get_n_monitors(display);
				assert_test(monitor_count != 0);
				monitor = gdk_display_get_monitor(display, 0);
				assert_test(monitor != nullptr);
			}
			return monitor;
		}

		auto get_current_window(
		) const -> GtkWindow * {
			auto window = GTK_WINDOW(g_list_nth_data(gtk_application_get_windows(thiz.m_application), 0));
			assert_test(window != nullptr);
			return window;
		}

		#pragma endregion

	};

}
