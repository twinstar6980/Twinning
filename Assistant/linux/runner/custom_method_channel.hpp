#pragma once

#include <gtk/gtk.h>
#include <flutter_linux/flutter_linux.h>
#include "./common.hpp"

class CustomMethodChannel {

private:

	GtkApplication * m_host;

	FlMethodChannel * m_channel;

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
		GtkApplication * host
	) :
		m_host{host},
		m_channel{nullptr} {
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

	auto register_activate(
		GApplication * application,
		FlView *       view
	) -> void {
		g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
		thiz.m_channel = fl_method_channel_new(
			fl_engine_get_binary_messenger(fl_view_get_engine(view)),
			std::format("{}.CustomMethodChannel", thiz.query_application_identifier()).data(),
			FL_METHOD_CODEC(codec)
		);
		fl_method_channel_set_method_call_handler(
			thiz.m_channel,
			[](
				FlMethodChannel * channel,
				FlMethodCall *    method_call,
				gpointer          user_data
			) -> void {
				auto & self = *reinterpret_cast<CustomMethodChannel *>(user_data);
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

	auto register_dispose(
		GObject * object
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
			auto argument_map = fl_method_call_get_args(call);
			assert_test(fl_value_get_type(argument_map) == FL_VALUE_TYPE_MAP);
			auto result_map = fl_value_new_map();
			auto get_argument = [&]<typename TValue>(std::string_view const & name) -> TValue {
				auto value_f = fl_value_lookup_string(argument_map, name.data());
				auto value = TValue{};
				if constexpr (std::is_same_v<TValue, bool>) {
					assert_test(fl_value_get_type(value_f) == FL_VALUE_TYPE_BOOL);
					value = fl_value_get_bool(value_f);
				}
				if constexpr (std::is_same_v<TValue, std::string>) {
					assert_test(fl_value_get_type(value_f) == FL_VALUE_TYPE_STRING);
					value = std::string{fl_value_get_string(value_f)};
				}
				return value;
			};
			auto set_result = [&]<typename TValue>(std::string_view const & name, TValue const & value) -> void {
				auto value_f = std::add_pointer_t<FlValue>{nullptr};
				if constexpr (std::is_same_v<TValue, std::string>) {
					value_f = fl_value_new_string(value.data());
				}
				if constexpr (std::is_same_v<TValue, std::vector<std::string>>) {
					value_f = fl_value_new_list();
					for (auto & value_item : value) {
						fl_value_append(value_f, fl_value_new_string(value_item.data()));
					}
				}
				fl_value_set_string_take(result_map, name.data(), value_f);
				return;
			};
			switch (hash_string(fl_method_call_get_name(call))) {
				case hash_string("query_storage_item"): {
					auto detail = thiz.handle_query_storage_item(
						get_argument.operator ()<std::string>("type")
					);
					set_result("target", std::get<0>(detail));
					break;
				}
				case hash_string("reveal_storage_item"): {
					[[maybe_unused]]
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
			response = FL_METHOD_RESPONSE(fl_method_success_response_new(result_map));
		}
		catch (...) {
			response = FL_METHOD_RESPONSE(fl_method_error_response_new("", thiz.parse_current_exception().data(), nullptr));
		}
		return;
	}

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
		auto window = GTK_WINDOW(g_list_nth_data(gtk_application_get_windows(thiz.m_host), 0));
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

	#pragma endregion

	#pragma region utility

	auto query_application_identifier(
	) const -> std::string {
		return std::string{"com.twinstar.twinning.assistant"};
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
		catch (...) {
			message = "UnknownException";
		}
		return message;
	}

	auto open_link(
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
