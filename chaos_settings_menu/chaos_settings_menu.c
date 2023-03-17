#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

/*
 *
 * Welcome Page
 *
 */

/*
 *
 * Key Bindings
 *
 */


static GtkWidget*
keybindings(void)
{
	GtkWidget *keybindings_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *keybindings_hbox =
	gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 100);
	gtk_widget_set_halign (keybindings_hbox, GTK_ALIGN_CENTER);
	gtk_widget_set_valign (keybindings_hbox, GTK_ALIGN_CENTER);
	GtkWidget *label = gtk_label_new ("KEY PAIR");
	gtk_box_append ( GTK_BOX (keybindings_hbox), label);
	gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
	gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
	gtk_widget_set_hexpand (label, TRUE);
	gtk_widget_add_css_class (label, "title-1");
	label = gtk_label_new ("ACTION");
	gtk_box_append ( GTK_BOX (keybindings_hbox), label);
	gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
	gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
	gtk_widget_set_hexpand (label, TRUE);
	gtk_widget_add_css_class (label, "title-1");
	gtk_box_append ( GTK_BOX (keybindings_vbox), keybindings_hbox);
	const char *keys[] = {
		"Windows + Enter",
		"Launch a Terminal",
		"Windows + F1",
		"Lock Screen",
		"Windows + o",
		"Launch Rofi Menu",
		"Windows + p",
		"Launch Dashboard"
	};
	const char *actions[] = {
		"Windows + Enter",
		"Launch a Terminal",
		"Windows + F1",
		"Lock Screen",
		"Windows + o",
		"Launch Rofi Menu",
		"Windows + p",
		"Launch Dashboard"
	};
	int size = sizeof(keys) / sizeof(keys[0]);
	for (int idx = 0; idx < size; idx++) {
		GtkWidget* separator =
			gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
		gtk_box_append (GTK_BOX (keybindings_vbox), separator);
		GtkWidget *keybinding_pair_hbox =
			gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 100);
		gtk_widget_set_halign (keybinding_pair_hbox, GTK_ALIGN_CENTER);
		gtk_widget_set_valign (keybinding_pair_hbox, GTK_ALIGN_CENTER);
		GtkWidget *label = gtk_label_new (keys[idx]);
		gtk_box_append ( GTK_BOX (keybinding_pair_hbox), label);
		gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
		gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
		gtk_widget_set_hexpand (label, TRUE);
		gtk_widget_add_css_class (label, "title-4");
		label = gtk_label_new (actions[idx]);
		gtk_box_append ( GTK_BOX (keybinding_pair_hbox), label);
		gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
		gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
		gtk_widget_set_hexpand (label, TRUE);
		gtk_widget_add_css_class (label, "title-4");
		gtk_box_append(GTK_BOX(keybindings_vbox), keybinding_pair_hbox);
	}
	return keybindings_vbox;
}

/*
 *
 * Tint2 Configurations
 *
 */

static void
add_row(GtkGrid *table, int row, GtkSizeGroup *size_group, const char *label_text, const char  **options)
{
	GtkWidget *label;
	label = gtk_label_new_with_mnemonic (label_text);
	gtk_widget_set_halign (label, GTK_ALIGN_START);
	gtk_widget_set_valign (label, GTK_ALIGN_BASELINE);
	gtk_widget_set_hexpand (label, TRUE);
	gtk_grid_attach (table, label, 0, row, 1, 1);
	GtkColorDialog *menu_item_color_dialog = gtk_color_dialog_new ();
        GtkWidget* menu_item_color_dialog_button = gtk_color_dialog_button_new(menu_item_color_dialog);
	gtk_label_set_mnemonic_widget (GTK_LABEL (label), menu_item_color_dialog_button);
	gtk_widget_set_halign (menu_item_color_dialog_button, GTK_ALIGN_END);
	gtk_widget_set_valign (menu_item_color_dialog_button, GTK_ALIGN_BASELINE);
	gtk_size_group_add_widget (size_group, menu_item_color_dialog_button);
	gtk_grid_attach (table, menu_item_color_dialog_button, 1, row, 1, 1);
}

static char*
hex_to_rgb(char *hex);

static char*
rgb_to_hex(char *rgb);

static char*
tint2_panel_color(void)
{
	FILE *panel = popen("cat ~/.config/tint2/tint2rc | grep -A6 'Background 1: Panel' | grep 'background_color' | awk '{print $3}' | cut -d '#' -f2", "r");
	//FILE *panel = popen("cat ~/.config/tint2/tint2rc | grep -A6 'Background 1: Panel' | grep 'background_color' | awk '{print $3}'", "r");
	//char *hex = malloc(7);
	char hex[256];
	while (fgets(hex, sizeof(hex), panel));
	pclose(panel);
	char *p = strchr(hex, '\n'); *p = '\0';
	g_print(hex);
	char *color = hex;
	return color;
}


/*
 *
 * Network Settings
 *
 */

static char*
wired_interface(void)
{
	FILE *interface = popen("find /sys/class/net/*/phydev -type l | cut -d '/' -f5", "r");
	char eth[256];
	while (fgets(eth, sizeof(eth), interface));
	pclose(interface);
	char *p = strchr(eth, '\n'); *p = '\0';
	char *wired_interface = eth;
	return wired_interface;
}

static void
wired_names (GtkStringList *sl)
{
	char *interface = wired_interface ();
	char command[] = "nmcli device show ";
	strcat(command, interface);
	char *rest = " | grep 'GENERAL.CONNECTION:' | cut -d ':' -f2 | sed 's/^ *//'";
	strcat(command, rest);
	FILE *connection = popen(command, "r");
	char name[256];
	while (fgets(name, sizeof(name), connection)) {
		char *p = strchr(name, '\n'); *p = '\0';
		gtk_string_list_append (sl, name);
	}
	pclose(connection);
	if (!sl) gtk_string_list_append (sl, "No wired connection");
}

static gboolean
wired_state (void)
{
	char *interface = wired_interface ();
	char command[] = "cat /sys/class/net/";
	strcat(command, interface);
	char carrier[] = "/carrier";
	strcat(command, carrier);
	FILE *file = popen(command, "r");
	char state[256];
	char *key = "0";
	while (fgets(state, sizeof(state), file));
	pclose(file);
	char *p = strchr(state, '\n'); *p = '\0';
	if (strcmp(key, state))
		return FALSE;
	else
		return TRUE;
}

static gboolean
wireless_state (void)
{
	FILE *state = popen("nmcli radio wifi", "r");
	char *key = "disabled";
	char buffer[256];
	while (fgets(buffer, sizeof(buffer), state));
	pclose(state);
	char *p = strchr(buffer, '\n'); *p = '\0';
	if (strcmp(key, buffer))
		return FALSE;
	else
		return TRUE;
}

static void
nearby_devices (GtkStringList *sl)
{
	FILE *access_points = popen("nmcli --fields SSID device wifi list | tail -n +2", "r");
	char access_point[1024];
	while (fgets(access_point, sizeof(access_point), access_points) != NULL) {
		char *p = strchr(access_point, '\n');
		if (p) *p = '\0';
		gtk_string_list_append (sl, access_point);
	}
  pclose(access_points);
}

GtkStringList *wired_string_list;
GtkWidget *wired_dropdown_menu;
const char *wired_connections[] = {NULL};
static gboolean
set_wired_switch_state (GtkSwitch* switch_button,
			gboolean state,
			gpointer user_data)
{
	if (state) {
		gtk_switch_set_state (switch_button, state);
		wired_string_list = gtk_string_list_new((const char * const *)wired_connections);
		wired_names (wired_string_list);
		wired_dropdown_menu = gtk_drop_down_new(wired_string_list, NULL);
		gtk_drop_down_set_enable_search(wired_dropdown_menu, TRUE);
		g_object_set (wired_dropdown_menu,
			"margin-start",  50,
			"margin-top",    10,
			"margin-end",    50,
			"margin-bottom", 50,
			NULL );
		gtk_widget_show (wired_dropdown_menu);
	} else {
		gtk_switch_set_state (switch_button, FALSE);
		gtk_switch_set_active (switch_button, FALSE);
		gtk_widget_hide (wired_dropdown_menu);
		wired_string_list = gtk_string_list_new ((const char * const *) wired_connections);
		wired_dropdown_menu = gtk_drop_down_new(wired_string_list, NULL);
	}
	return TRUE;
}

GtkStringList *string_list;
GtkWidget *dropdown_menu;
const char *connections[] = {NULL};
static gboolean
set_wireless_switch_state (GtkSwitch* switch_button,
                  gboolean state,
                  gpointer user_data)
{
	if (state) {
		system("nmcli radio wifi on");
		gtk_switch_set_state (switch_button, state);
		string_list =  gtk_string_list_new ((const char * const *) connections);
		nearby_devices (string_list);
		dropdown_menu = gtk_drop_down_new(string_list, NULL);
		gtk_drop_down_set_enable_search (dropdown_menu, TRUE);
		g_object_set ( dropdown_menu,
			"margin-start",  50,
			"margin-top",    10,
			"margin-end",    50,
			"margin-bottom", 50,
			NULL );
		gtk_widget_show (dropdown_menu);
	} else {
		system("nmcli radio wifi off");
		gtk_switch_set_state (switch_button, FALSE);
		gtk_switch_set_active (switch_button, FALSE);
		gtk_widget_hide (dropdown_menu);
		string_list =  gtk_string_list_new((const char* const*)connections);
		dropdown_menu = gtk_drop_down_new(string_list, NULL);
	}
	return TRUE;
}

/*
 *
 *  Display Settings
 *
 */

struct Monitor
{
	char *name;
	char *interface;
	char *status;
	char *screen_resolution;
	char *refresh_rate;
	char *orientation;
	char *position;
	char *brightness;

};

const char *attached_monitors[] = {NULL};
GtkStringList *attached_monitors_string_list;
static int
display_settings_monitor_count(void)
{
	int count;
	char monitors[2048];
	FILE *process = popen("xrandr --listmonitors | head -n 1 | awk '{print $2}'", "r");
	while (fgets(monitors, sizeof(monitors), process) != NULL) {
		char *p = strchr(monitors, '\n');
		if (p) *p = '\0';
	}
	pclose(process);
	sscanf(monitors, "%d", &count);
	return count;
}

static void
display_settings_monitors(GtkListStore *store)
{
	GtkTreeIter iter;
	char monitor[2048];
	int idx, index = 0, count = display_settings_monitor_count();
	attached_monitors_string_list = gtk_string_list_new((const char* const*)attached_monitors);
	FILE *process = popen("xrandr --listmonitors | tail -n +2", "r");
	while (fgets(monitor, sizeof(monitor), process) != NULL) {
		char *p = strchr(monitor, '\n');
		if (p) *p = '\0';
		gtk_string_list_append(attached_monitors_string_list, monitor);
		attached_monitors[index] = monitor;
		index++;
	}
	pclose(process);
	//sscanf()
	//const char *text[] = { "Red", "Green", "Blue", "Yellow" };
	//int i;

	/* First clear the store */
	//gtk_list_store_clear (store);
	//for (idx = 0; idx < count; idx++) {
	//	gtk_list_store_append (store, &iter);
	//	gtk_list_store_set (store, &iter, COL_TEXT, attached_monitors[idx], -1);
	//	printf(attached_monitors[idx]);
	//}
}

void
display_settings_monitor_clicked (GtkButton* self, gpointer user_data)
{
	GtkWidget *monitor_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);


}

/*
 *
 * Volume & Audio Settings
 *
 */

struct OutputDevice
{
	int sink_id;
	char *device_name;
};

const char *output_devices[] = {NULL};
const int *output_devices_numbers = {NULL};
GtkStringList *output_devices_string_list;
GtkStringList *output_devices_numbers_string_list;
struct OutputDevice output_devices_map[2048];
static gboolean
is_output_device_muted(char *id)
{
	char command[2048], state[2048];
	sprintf(command, "pamixer --sink %s --get-mute", id);
	FILE *process = popen(command, "r");
	while (fgets(state, sizeof(state), process) != NULL) {
		char *p = strchr(state, '\n');
		if (p) *p = '\0';
	}
	pclose(process);
	return strcmp("true", state) ? FALSE : TRUE;
}

static int
get_output_volume(char *id)
{
	int volume;
	char state[2048], command[2048];
	sprintf(command, "pamixer --sink %s --get-volume", id);
	FILE *process = popen(command, "r");
	while (fgets(state, sizeof(state), process) != NULL) {
		char *p = strchr(state, '\n');
		if (p) *p = '\0';
	}
	pclose(process);
	sscanf(state, "%d", &volume);
	return volume;
}

static int
output_devices_list_size(void)
{
	int count;
	char buffer[2048];
	FILE *command = popen("pamixer --list-sinks | tail -n +2 | wc -l", "r");
	while (fgets(buffer, sizeof(buffer), command) != NULL) {
		char *p = strchr(buffer, '\n');
		if (p) *p = '\0';
	}
	pclose(command);
	sscanf(buffer, "%d", &count);
	return count;
}

void
output_value_changed(GtkRange* self, gpointer user_data)
{
	char *id = user_data, volume[2048], command[2048];
	int new_value = gtk_range_get_value(self);
	sprintf(volume, "%d", new_value);
	sprintf(command, "pamixer --sink %s --set-volume %s", id, volume);
	system(command);
}

//void
//output_mute_button_clicked(GtkButton* self, gpointer user_data)
//{
//	char *id = user_data;
//	//int index
//	if (is_output_device_muted(gtk_string_list_get_string(output_devices_numbers_string_list, id))) {
//		char command[2048];
//		sprintf(command, "pamixer --sink %s --unmute", id);
//		gtk_button_set_label(self, " ");
//		system(command);
//	} else {
//		char command[2048];
//		sprintf(command, "pamixer --sink %s --mute", id);
//		gtk_button_set_label(self, "");
//		system(command);
//	}
//}

static GtkWidget*
output_devices_list (void)
{
	int size = output_devices_list_size();
	char sink_no[2048], sink_name[2048];
	GtkWidget *output_devices_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	gtk_widget_set_valign(output_devices_box, GTK_ALIGN_START);
	GtkWidget *output_devices_label = gtk_label_new("Output Devices");
	gtk_widget_set_valign(output_devices_label, GTK_ALIGN_START);
	gtk_widget_set_halign(output_devices_label, GTK_ALIGN_START);
	gtk_box_append(GTK_BOX(output_devices_box), output_devices_label);
	//pamixer --list-sinks | tail -n +2 | awk '{ print substr($0, index($0,$4)) }' | tr -d '"'
	gtk_box_set_homogeneous(GTK_BOX(output_devices_box), TRUE);
	output_devices_string_list = gtk_string_list_new((const char* const*)output_devices);
	output_devices_numbers_string_list = gtk_string_list_new((const char* const*)output_devices_numbers);
	FILE *sink_numbers = popen("pamixer --list-sinks | tail -n +2 | awk '{ print $1 }'", "r");
	while (fgets(sink_no, sizeof(sink_no), sink_numbers) != NULL) {
		char *p = strchr (sink_no, '\n');
		if (p) *p = '\0';
		gtk_string_list_append(output_devices_numbers_string_list, sink_no);
	}
	pclose(sink_numbers);
	FILE *sink_names = popen("pamixer --list-sinks | tail -n +2 | awk '{ print substr($0, index($0,$4)) }' | tr -d '\"'", "r");
	//if (!command)
	//    return NULL;
	while (fgets(sink_name, sizeof(sink_name), sink_names) != NULL) {
		char *p = strchr(sink_name, '\n');
		if (p) *p = '\0';
		gtk_string_list_append(output_devices_string_list, sink_name);
	}
	pclose(sink_names);
	for (int idx = 0; idx < size; idx++) {
		int no;
		sscanf(gtk_string_list_get_string(output_devices_numbers_string_list, idx), "%d", &no);
		output_devices_map[idx].sink_id = no;
	        char *name = gtk_string_list_get_string(output_devices_string_list, idx);
		output_devices_map[idx].device_name = name;
		//printf("Sink id: %d\n", output_devices_map[idx].sink_id);
		//printf("Sink name: %s\n", output_devices_map[idx].device_name);
	}
	for (int idx = 0; idx < size; idx++) {
		GtkWidget *output_device_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
		gtk_widget_set_valign (output_device_vbox, GTK_ALIGN_START);

		GtkWidget *output_device_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 60);
		//gtk_widget_set_halign (output_device_hbox, GTK_ALIGN_CENTER);

		GtkWidget *output_device_name = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
		//gtk_widget_set_halign (output_device_name, GTK_ALIGN_START);

		GtkWidget *output_device_features = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
		//gtk_widget_set_halign (output_device_features, GTK_ALIGN_END);

		char *output_device = gtk_string_list_get_string(output_devices_string_list, idx);

		GtkWidget *output_device_label = gtk_label_new(output_device);

		GtkWidget *output_device_mute_button = gtk_button_new();
		if (is_output_device_muted(gtk_string_list_get_string(output_devices_numbers_string_list, idx)))
			gtk_button_set_label(output_device_mute_button, "");
		else
			gtk_button_set_label(output_device_mute_button, " ");

		//g_signal_connect(output_device_mute_button, "clicked", G_CALLBACK(output_mute_button_clicked), gtk_string_list_get_string(output_devices_numbers_string_list, idx));
		GtkWidget *output_device_default_button = gtk_button_new_with_label("Default");

		gtk_box_append (GTK_BOX (output_device_name), output_device_label);
		gtk_widget_set_halign (output_device_name, GTK_ALIGN_START);
		gtk_widget_set_valign (output_device_name, GTK_ALIGN_START);

		gtk_box_append (GTK_BOX (output_device_features), output_device_mute_button);

		gtk_box_append (GTK_BOX (output_device_features), output_device_default_button);
		gtk_widget_set_halign (output_device_features, GTK_ALIGN_END);

		gtk_box_append (GTK_BOX (output_device_hbox), output_device_name);

		gtk_box_append (GTK_BOX (output_device_hbox), output_device_features);
		gtk_widget_set_halign (output_device_hbox, GTK_ALIGN_CENTER);

		//gtk_box_append (GTK_BOX (output_device_hbox), output_device_default_button);
		GtkWidget *output_device_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 153.0, 1.0);

		int volume = get_output_volume(gtk_string_list_get_string(output_devices_numbers_string_list, idx));
		gtk_range_set_value(output_device_scale, volume);
		gtk_scale_set_draw_value(output_device_scale, volume);

		g_signal_connect(output_device_scale, "value-changed", G_CALLBACK(output_value_changed), gtk_string_list_get_string(output_devices_numbers_string_list, idx));
		GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
		//gtk_box_append (GTK_BOX (output_device_hbox), output_device_name);
		gtk_box_append(GTK_BOX(output_device_vbox), output_device_hbox);
		gtk_box_append(GTK_BOX(output_device_vbox), output_device_scale);
		gtk_box_append(GTK_BOX(output_device_vbox), separator);
		gtk_widget_set_valign(output_device_vbox, GTK_ALIGN_START);
		gtk_box_append(GTK_BOX (output_devices_box), output_device_vbox);
	}

	return output_devices_box;
}

struct InputDevice
{
	int source_id;
	char *device_name;
};

const char *input_devices[] = {NULL};
const int *input_devices_numbers = {NULL};
GtkStringList *input_devices_string_list;
GtkStringList *input_devices_numbers_string_list;
struct InputDevice input_devices_map[2048];
static gboolean
is_input_device_muted(char *id)
{
	char command[2048], state[2048];
	sprintf(command, "pamixer --source %s --get-mute", id);
	FILE *process = popen(command, "r");
	while (fgets(state, sizeof(state), process) != NULL) {
		char *p = strchr(state, '\n');
		if (p) *p = '\0';
	}
	pclose(process);
	return strcmp("true", state) ? FALSE : TRUE;
}

static int
get_input_volume(char *id)
{
	int volume;
	char state[2048], command[2048];
	sprintf(command, "pamixer --source %s --get-volume", id);
	FILE *process = popen(command, "r");
	while (fgets(state, sizeof(state), process) != NULL) {
		char *p = strchr(state, '\n');
		if (p) *p = '\0';
	}
	pclose(process);
	sscanf(state, "%d", &volume);
	return volume;
}

static int
input_devices_list_size(void)
{
	int count;
	char buffer[2048];
	FILE *command = popen("pamixer --list-sources | tail -n +2 | wc -l", "r");
	while (fgets(buffer, sizeof(buffer), command) != NULL) {
		char *p = strchr(buffer, '\n');
		if (p) *p = '\0';
	}
	pclose(command);
	sscanf(buffer, "%d", &count);
	return count;
}

void
input_value_changed(GtkRange* self, gpointer user_data)
{
	char *id = user_data, volume[2048], command[2048];
	int new_value = gtk_range_get_value(self);
	sprintf(volume, "%d", new_value);
	sprintf(command, "pamixer --source %s --set-volume %s", id, volume);
	system(command);
}

//void
//input_mute_button_clicked(GtkButton* self, gpointer user_data)
//{
//	char *id = user_data;
//	//int index
//	if (is_input_device_muted(gtk_string_list_get_string(input_devices_numbers_string_list, id))) {
//		char command[2048];
//		sprintf(command, "pamixer --source %s --unmute", id);
//		gtk_button_set_label(self, " ");
//		system(command);
//	} else {
//		char command[2048];
//		sprintf(command, "pamixer --source %s --mute", id);
//		gtk_button_set_label(self, "");
//		system(command);
//	}
//}

static GtkWidget*
input_devices_list (void)
{
	int size = input_devices_list_size();
	char source_no[2048], source_name[2048];
	GtkWidget *input_devices_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	gtk_widget_set_valign(input_devices_box, GTK_ALIGN_START);
	GtkWidget *input_devices_label = gtk_label_new("Input Devices");
	gtk_widget_set_valign(input_devices_label, GTK_ALIGN_START);
	gtk_widget_set_halign(input_devices_label, GTK_ALIGN_START);
	gtk_box_append(GTK_BOX(input_devices_box), input_devices_label);
	//pamixer --list-sources | tail -n +2 | awk '{ print substr($0, index($0,$4)) }' | tr -d '"'
	gtk_box_set_homogeneous(GTK_BOX(input_devices_box), TRUE);
	input_devices_string_list = gtk_string_list_new((const char* const*)input_devices);
	input_devices_numbers_string_list = gtk_string_list_new((const char* const*)input_devices_numbers);
	FILE *source_numbers = popen("pamixer --list-sources | tail -n +2 | awk '{ print $1 }'", "r");
	while (fgets(source_no, sizeof(source_no), source_numbers) != NULL) {
		char *p = strchr (source_no, '\n');
		if (p) *p = '\0';
		gtk_string_list_append(input_devices_numbers_string_list, source_no);
	}
	pclose(source_numbers);
	FILE *source_names = popen("pamixer --list-sources | tail -n +2 | awk '{ print substr($0, index($0,$4)) }' | tr -d '\"'", "r");
	//if (!command)
	//    return NULL;
	while (fgets(source_name, sizeof(source_name), source_names) != NULL) {
		char *p = strchr(source_name, '\n');
		if (p) *p = '\0';
		gtk_string_list_append(input_devices_string_list, source_name);
	}
	pclose(source_names);
	for (int idx = 0; idx < size; idx++) {
		int no;
		sscanf(gtk_string_list_get_string(input_devices_numbers_string_list, idx), "%d", &no);
		input_devices_map[idx].source_id = no;
	        char *name = gtk_string_list_get_string(input_devices_string_list, idx);
		input_devices_map[idx].device_name = name;
	}
	for (int idx = 0; idx < size; idx++) {
		GtkWidget *input_device_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
		gtk_widget_set_valign (input_device_vbox, GTK_ALIGN_START);

		GtkWidget *input_device_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 60);
		//gtk_widget_set_halign (input_device_hbox, GTK_ALIGN_CENTER);

		GtkWidget *input_device_name = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
		//gtk_widget_set_halign (input_device_name, GTK_ALIGN_START);

		GtkWidget *input_device_features = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
		//gtk_widget_set_halign (input_device_features, GTK_ALIGN_END);

		char *input_device = gtk_string_list_get_string(input_devices_string_list, idx);

		GtkWidget *input_device_label = gtk_label_new(input_device);

		GtkWidget *input_device_mute_button = gtk_button_new();
		if (is_input_device_muted(gtk_string_list_get_string(input_devices_numbers_string_list, idx)))
			gtk_button_set_label(input_device_mute_button, "");
		else
			gtk_button_set_label(input_device_mute_button, " ");

		//g_signal_connect(input_device_mute_button, "clicked", G_CALLBACK(input_mute_button_clicked), gtk_string_list_get_string(input_devices_numbers_string_list, idx));
		GtkWidget *input_device_default_button = gtk_button_new_with_label("Default");

		gtk_box_append (GTK_BOX (input_device_name), input_device_label);
		gtk_widget_set_halign (input_device_name, GTK_ALIGN_START);
		gtk_widget_set_valign (input_device_name, GTK_ALIGN_START);

		gtk_box_append (GTK_BOX (input_device_features), input_device_mute_button);

		gtk_box_append (GTK_BOX (input_device_features), input_device_default_button);
		gtk_widget_set_halign (input_device_features, GTK_ALIGN_END);

		gtk_box_append (GTK_BOX (input_device_hbox), input_device_name);

		gtk_box_append (GTK_BOX (input_device_hbox), input_device_features);
		gtk_widget_set_halign (input_device_hbox, GTK_ALIGN_CENTER);

		//gtk_box_append (GTK_BOX (input_device_hbox), input_device_default_button);
		GtkWidget *input_device_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 153.0, 1.0);

		int volume = get_input_volume(gtk_string_list_get_string(input_devices_numbers_string_list, idx));
		gtk_range_set_value(input_device_scale, volume);
		gtk_scale_set_draw_value(input_device_scale, volume);

		g_signal_connect(input_device_scale, "value-changed", G_CALLBACK(input_value_changed), gtk_string_list_get_string(input_devices_numbers_string_list, idx));
		GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
		//gtk_box_append (GTK_BOX (input_device_hbox), input_device_name);
		gtk_box_append(GTK_BOX(input_device_vbox), input_device_hbox);
		gtk_box_append(GTK_BOX(input_device_vbox), input_device_scale);
		gtk_box_append(GTK_BOX(input_device_vbox), separator);
		gtk_widget_set_valign(input_device_vbox, GTK_ALIGN_START);
		gtk_box_append(GTK_BOX (input_devices_box), input_device_vbox);
	}

	return input_devices_box;
}


/*
 *
 *
 * Time & Language Settings
 *
 */

static char*
current_timezone(void)
{
	char buffer[2048];
	FILE *command = popen("readlink /etc/localtime | cut -d '/' -f5-", "r");
	while (fgets (buffer, sizeof (buffer), command) != NULL) {
		char *p = strchr (buffer, '\n');
		if (p) *p = '\0';
	}
	pclose(command);
	char *current_localtime = buffer;
	return current_localtime;
}

static int
current_timezone_position(void)
{
	int position;
	char full_command[2048];
	char *command = "find /usr/share/zoneinfo/ -type f -exec grep -IL . \"{}\" \\; | cut -d '/' -f5- | sort | grep -nx \'";
	char *current_localtime = current_timezone ();
	char *rest_of_command = "\' | cut -d ':' -f1";
	strcpy (full_command, command);
	strcat (full_command, current_localtime);
	strcat (full_command, rest_of_command);
	FILE *process = popen (full_command, "r");
	char buffer[2048];
	while (fgets (buffer, sizeof (buffer), process) != NULL) {
		char *p = strchr (buffer, '\n');
		if (p) *p = '\0';
	}
	pclose(process);
	sscanf(buffer, "%d", &position);
	return position;
}

const char *timezones[] = {NULL};
GtkStringList *timezone_string_list;
GtkWidget *timezone_dropdown_menu;
static GtkWidget*
select_timezone(void)
{
	GtkWidget *timezone_box, *label;
	timezone_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 15);
	gtk_box_set_homogeneous (GTK_BOX (timezone_box), TRUE);
	label = gtk_label_new ("Select a timezone:");
	gtk_box_append (GTK_BOX (timezone_box), label);
	timezone_string_list = gtk_string_list_new((const char* const*)timezones);
	//FILE *command = popen ("find /usr/share/zoneinfo -print | cut -d '/' -f5- | sort | nl", "r");
	FILE *command = popen ("find /usr/share/zoneinfo/ -type f -exec grep -IL . \"{}\" \\; | cut -d '/' -f5- | sort | nl", "r");
	//if (!command)
	//    return NULL;
	char buffer[2048];
	int position = current_timezone_position ();
	while (fgets (buffer, sizeof (buffer), command) != NULL) {
		char *p = strchr (buffer, '\n');
		if (p) *p = '\0';
		gtk_string_list_append(timezone_string_list, buffer);
	}
	pclose(command);
	//gtk_string_list_remove (timezone_string_list, 0);
	timezone_dropdown_menu = gtk_drop_down_new(timezone_string_list, NULL);
	gtk_drop_down_set_enable_search (timezone_dropdown_menu, TRUE);
	g_object_set (timezone_dropdown_menu,
		"margin-start",  100,
                //"margin-top",    50,
                "margin-end",    100,
                //"margin-bottom", 50,
                NULL );
	gtk_drop_down_set_selected (timezone_dropdown_menu, position - 1);
	gtk_box_append (GTK_BOX (timezone_box), timezone_dropdown_menu);
	return timezone_box;
}

static void
timezone_save_button_clicked(GtkWidget *widget, gpointer data)
{
	guint selected_timezone_position = gtk_drop_down_get_selected(
					GTK_DROP_DOWN(timezone_dropdown_menu));
	int position = selected_timezone_position, idx = 0;
	char *timezone, *selected_timezone =
		gtk_string_list_get_string(timezone_string_list, position);
	while (selected_timezone[idx] != '\0') {
		if (selected_timezone[idx] <= 32 &&
		    selected_timezone[idx + 1] > 32)
			timezone = &selected_timezone[idx + 1];
		idx++;
	}
	char command[2048];
	sprintf(command, "ln -sf /usr/share/zoneinfo/%s /tmp/localtime", timezone);
	//sprintf(command, "ln -sf /usr/share/zoneinfo/%s /etc/localtime", last_word);
	system(command);
	//GtkWidget *popover = gtk_popover_new ();
	//GtkWidget *popover_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
	//gtk_popover_set_child (GTK_POPOVER (popover), popover_box);
	//gtk_box_append (GTK_BOX (popover_box), gtk_label_new ("Timezone successfully changed."));
	//gtk_popover_set_autohide (popover, TRUE);
	//popover = gtk_popover_new (button);
	//gtk_container_add (popover, popover_box);
	//gtk_menu_button_set_popover (GTK_MENU_BUTTON (button), popover);
	//gtk_widget_show (popover);
}
/*
 *
 * Applications
 *
 */

static GListModel *
create_application_list (void)
{
	GListStore *store;
	GList *apps, *l;
	store = g_list_store_new (G_TYPE_APP_INFO);
	apps = g_app_info_get_all ();
	for (l = apps; l; l = l->next)
		g_list_store_append (store, l->data);
	g_list_free_full (apps, g_object_unref);
	return G_LIST_MODEL (store);
}

static void
setup_listitem_cb (GtkListItemFactory *factory,
                   GtkListItem *list_item)
{
	GtkWidget *apps_box;
	GtkWidget *image;
	GtkWidget *label;
	apps_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
	image = gtk_image_new ();
	gtk_image_set_icon_size (GTK_IMAGE (image), GTK_ICON_SIZE_LARGE);
	gtk_box_append (GTK_BOX (apps_box), image);
	label = gtk_label_new ("");
	gtk_box_append (GTK_BOX (apps_box), label);
	gtk_list_item_set_child (list_item, apps_box);
}

static void
bind_listitem_cb (GtkListItemFactory *factory,
                  GtkListItem        *list_item)
{
	GtkWidget *image;
	GtkWidget *label;
	GAppInfo *app_info;
	image = gtk_widget_get_first_child(gtk_list_item_get_child (list_item));
	label = gtk_widget_get_next_sibling (image);
	app_info = gtk_list_item_get_item(list_item);
	gtk_image_set_from_gicon(GTK_IMAGE(image),
				 g_app_info_get_icon(app_info));
	gtk_label_set_label(GTK_LABEL(label),
			    g_app_info_get_display_name(app_info));
}

static void
activate_cb (GtkListView  *list,
             guint position,
             gpointer unused)
{
	GAppInfo *app_info;
	GdkAppLaunchContext *context;
	GError *error = NULL;
	app_info = g_list_model_get_item(G_LIST_MODEL(gtk_list_view_get_model(list)),position);
	context = gdk_display_get_app_launch_context(gtk_widget_get_display(GTK_WIDGET (list)));
	if (!g_app_info_launch (app_info, NULL, G_APP_LAUNCH_CONTEXT (context),
				&error)) {
		GtkWidget *dialog;
		dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET (list))),
                                       GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                       GTK_MESSAGE_ERROR,
                                       GTK_BUTTONS_CLOSE,
                                       "Could not launch %s", g_app_info_get_display_name (app_info));
		gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog), "%s", error->message);
		g_clear_error (&error);
		gtk_widget_show (dialog);
	}
	g_object_unref (context);
	g_object_unref (app_info);
}

/*
 *
 * Software Updates & Info. Security
 *
 */

static GtkWidget *password_entry;
static GtkWidget *password_button;

static gboolean
check_password()
{
	int error;
	char command[2048];
	sprintf(command, "echo %s | sudo -S pacman -Syup", password_entry);
	if (error = system(command))
		return FALSE;
	return TRUE;
}

static void
password_button_clicked(GtkButton* self, gpointer user_data)
{
	if (check_password()) {
		printf("correct");
	} else {
		printf("wrong");
	}
	printf(password_entry);
}


static GtkWidget*
password_for_updates(void)
{
	GtkWidget *box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
	gtk_widget_set_margin_start (box, 18);
	gtk_widget_set_margin_end (box, 18);
	gtk_widget_set_margin_top (box, 18);
	gtk_widget_set_margin_bottom (box, 18);
	//gtk_window_set_child (GTK_WINDOW (window), box);

	password_entry = gtk_password_entry_new ();
	setuid(0); setgid(0);
	gtk_password_entry_set_show_peek_icon (GTK_PASSWORD_ENTRY (password_entry), TRUE);
	g_object_set (password_entry,
                    "placeholder-text", "Password",
                    "activates-default", TRUE,
                    NULL);
	g_signal_connect (password_entry, "notify::text", G_CALLBACK (password_button), NULL);
	gtk_box_append (GTK_BOX (box), password_entry);
	password_button = gtk_button_new_with_mnemonic ("_Done");
	gtk_widget_add_css_class (password_button, "suggested-action");
	g_signal_connect (password_button, "clicked", G_CALLBACK (password_button_clicked), NULL);
	gtk_widget_set_sensitive (password_button, TRUE);
	gtk_box_append (GTK_BOX (box), password_button);
	char command[2048]; sprintf(command, "echo 'password %s' > /tmp/password.txt", password_entry);
	system(command);
	
	return box;
}

//TODO

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
	static GtkWidget *window = NULL;
	static GtkWidget *apps_window = NULL;
	GtkWidget *sidebar;
	GtkWidget *stack;
	GtkWidget *box;
	GtkWidget *widget;
	GtkWidget *header;
	GtkWidget *main_box;
	GtkWidget *main_label;
	GtkWidget *bindings_vbox;
	GtkWidget *bindings_table;
	GtkWidget *tint2_vbox;
	GtkWidget *table, *label, *picker;
	GtkWidget *volume_box;
	#define COLOR(r,g,b) { r/255., g/255., b/255., 1.0 }
	GdkRGBA solarized[] = {
		COLOR (0xff, 0xff, 0xff),
		COLOR (0x07, 0x36, 0x42),
		COLOR (0xdc, 0x32, 0x2f),
		COLOR (0x85, 0x99, 0x00),
		COLOR (0xb5, 0x89, 0x00),
		COLOR (0x26, 0x8b, 0xd2),
		COLOR (0xd3, 0x36, 0x82),
		COLOR (0x2a, 0xa1, 0x98),
		COLOR (0xee, 0xe8, 0xd5),
		COLOR (0x00, 0x00, 0x00),
		COLOR (0x00, 0x2b, 0x36),
		COLOR (0xcb, 0x4b, 0x16),
		COLOR (0x58, 0x6e, 0x75),
		COLOR (0x65, 0x7b, 0x83),
		COLOR (0x83, 0x94, 0x96),
		COLOR (0x6c, 0x71, 0xc4),
		COLOR (0x93, 0xa1, 0xa1),
		COLOR (0xfd, 0xf6, 0xe3),
	};
	const char * pages[] = {
		"Welcome to ChaOS Settings",
		"Key Bindings",
		"Tint2 Configurations",
		"System Settings",
		"Network Settings",
		"Display Settings",
		"Volume & Audio Settings",
		"Time & Language Settings",
		"Applications",
		"Software Updates & Info. Security",
		NULL
	};
	const char *c = NULL;
	guint i;

	if (!window) {
		window = gtk_application_window_new (app);
		gtk_window_set_resizable (GTK_WINDOW (window), TRUE);
		header = gtk_header_bar_new ();
		gtk_window_set_titlebar (GTK_WINDOW(window), header);
		gtk_window_set_title (GTK_WINDOW(window), "ChaOS Settings Menu");
		g_object_add_weak_pointer (G_OBJECT (window), (gpointer *)&window);
		box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
		sidebar = gtk_stack_sidebar_new ();
		gtk_box_append (GTK_BOX (box), sidebar);
		stack = gtk_stack_new ();
		gtk_stack_set_transition_type (GTK_STACK (stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);
		gtk_stack_sidebar_set_stack (GTK_STACK_SIDEBAR (sidebar), GTK_STACK (stack));
		gtk_widget_set_hexpand (stack, TRUE);
		gtk_box_append (GTK_BOX (box), stack);
		for (i=0; (c = *(pages+i)) != NULL; i++ ) {
			if (i == 0) {
				main_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 25);
				gtk_widget_set_valign (main_box, GTK_ALIGN_CENTER);
				gtk_widget_set_halign (main_box, GTK_ALIGN_CENTER);
				widget = gtk_image_new_from_file("we_are_fsociety.png");
				gtk_widget_add_css_class (widget, "icon-dropshadow");
				gtk_image_set_pixel_size (GTK_IMAGE (widget), 500);
				gtk_box_append (GTK_BOX (main_box), widget);
				main_label = gtk_label_new("chaOS - Settings & Usage Guide");
				gtk_box_append (GTK_BOX (main_box), main_label);
				gtk_stack_add_named (GTK_STACK (stack), main_box, c);
				g_object_set (gtk_stack_get_page (GTK_STACK (stack),
						main_box), "title", c, NULL);
			} else if (i == 1) {
				bindings_vbox = keybindings ();
				//bindings_vbox = gtk_box_new ( GTK_ORIENTATION_VERTICAL, 10);
				//GtkWidget *bindings_hbox = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 5);
				//bindings_table = gtk_grid_new ();
				//gtk_box_append ( GTK_BOX (bindings_hbox), bindings_table);
				//gtk_widget_set_margin_start (bindings_table, 20);
				//gtk_widget_set_margin_end (bindings_table, 20);
				//gtk_widget_set_margin_top (bindings_table, 20);
				//gtk_widget_set_margin_bottom (bindings_table, 20);
				//gtk_grid_set_row_spacing ( GTK_GRID (bindings_table), 3);
				//gtk_grid_set_column_spacing ( GTK_GRID (bindings_table), 10);
				//gtk_window_set_child ( GTK_WINDOW (window), table);
				//label = gtk_label_new ("KEY PAIR");
				//gtk_box_append ( GTK_BOX (bindings_hbox), label);
				//gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
				//gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
				//gtk_widget_set_hexpand (label, TRUE);
				//gtk_widget_add_css_class (label, "title-4");
				//gtk_grid_attach (GTK_GRID (bindings_table), label, 0, 0, 1, 1);
				//label = gtk_label_new ("ACTION");
				//gtk_box_append ( GTK_BOX (bindings_hbox), label);
				//gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
				//gtk_widget_set_valign (label, GTK_ALIGN_CENTER);
				//gtk_widget_set_hexpand (label, TRUE);
				//gtk_widget_add_css_class (label, "title-4");
				//gtk_grid_attach (GTK_GRID (bindings_table), label, 1, 0, 1, 1);
				//gtk_box_append (GTK_BOX (bindings_vbox), bindings_hbox);
				gtk_stack_add_named (GTK_STACK(stack),
					     bindings_vbox, c);
				g_object_set (gtk_stack_get_page (GTK_STACK(stack),
					bindings_vbox), "title", c, NULL);
			} else if (i == 2) {
				//GtkWidget *tint2_vbox = tint2_configuration();
				GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
				gtk_widget_set_margin_start (vbox, 5);
				gtk_widget_set_margin_end (vbox, 5);
				gtk_widget_set_margin_top (vbox, 5);
				gtk_widget_set_margin_bottom (vbox, 5);
				GtkSizeGroup *size_group = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
				GtkSizeGroupMode new_mode = GTK_SIZE_GROUP_HORIZONTAL;
				g_object_set_data_full (G_OBJECT (window), "size-group", size_group, g_object_unref);
				GtkWidget *frame = gtk_frame_new ("Panel Options");
				gtk_box_append (GTK_BOX (vbox), frame);
				GtkWidget *table = gtk_grid_new ();
				gtk_widget_set_margin_start (table, 5);
				gtk_widget_set_margin_end (table, 5);
				gtk_widget_set_margin_top (table, 5);
				gtk_widget_set_margin_bottom (table, 5);
				gtk_grid_set_row_spacing (GTK_GRID (table), 5);
				gtk_grid_set_column_spacing (GTK_GRID (table), 10);
				gtk_frame_set_child (GTK_FRAME (frame), table);
				GtkColorDialog *menu_item_color_dialog = gtk_color_dialog_new ();
				GtkWidget* menu_item_color_dialog_button = gtk_color_dialog_button_new(menu_item_color_dialog);
				add_row (GTK_GRID (table), 0, size_group, "_Panel Background Color:", menu_item_color_dialog_button);
				add_row (GTK_GRID (table), 1, size_group, "_Panel Text Color:", menu_item_color_dialog_button);

				frame = gtk_frame_new ("Workstation Options");
				gtk_box_append (GTK_BOX (vbox), frame);
				table = gtk_grid_new ();
				gtk_widget_set_margin_start (table, 5);
				gtk_widget_set_margin_end (table, 5);
				gtk_widget_set_margin_top (table, 5);
				gtk_widget_set_margin_bottom (table, 5);
				gtk_grid_set_row_spacing (GTK_GRID (table), 5);
				gtk_grid_set_column_spacing (GTK_GRID (table), 10);
				gtk_frame_set_child (GTK_FRAME (frame), table);
				//GtkColorDialog *menu_item_color_dialog = gtk_color_dialog_new ();
				//GtkWidget* menu_item_color_dialog_button = gtk_color_dialog_button_new(menu_item_color_dialog);
				add_row (GTK_GRID (table), 0, size_group, "_Foreground", menu_item_color_dialog_button);
				add_row (GTK_GRID (table), 1, size_group, "_Background", menu_item_color_dialog_button);


				GdkRGBA *panel_custom_color_rgba;
				gdk_rgba_parse(panel_custom_color_rgba, "\\#aa7733");

				gtk_stack_add_named (GTK_STACK (stack), vbox, c);
				g_object_set (gtk_stack_get_page (GTK_STACK(stack),
						vbox), "title", c, NULL);
			} else if (i == 3) {
				widget = gtk_label_new (c);
				gtk_stack_add_named (GTK_STACK (stack), widget, c);
				g_object_set (gtk_stack_get_page (GTK_STACK(stack),
						widget), "title", c, NULL);
			} else if (i == 4) {
				GtkWidget *network_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
				GtkWidget *wired_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 35); 
				g_object_set (wired_box,
					"margin-start",  50,
					"margin-top",    30,
					"margin-end",    50,
					"margin-bottom", 10,
					NULL);
				GtkWidget *wireless_box = gtk_box_new(
					 GTK_ORIENTATION_HORIZONTAL, 25);
				GtkWidget *wired_switch = gtk_switch_new ();
				GtkWidget *wireless_switch = gtk_switch_new ();
				g_signal_connect(wired_switch, "state-set",
					 G_CALLBACK(set_wired_switch_state),
					 gtk_switch_get_active(wired_switch));
				g_signal_connect(wireless_switch, "state-set",
					 G_CALLBACK(set_wireless_switch_state),
					 gtk_switch_get_active(wireless_switch)
					 );
				g_object_set (wired_switch,
					"margin-start",  55,
					"margin-top",    10,
					"margin-end",    50,
					"margin-bottom", 5,
					NULL);
				g_object_set (wireless_switch,
				"margin-start",  50,
				"margin-top",    5,
				"margin-end",    50,
				"margin-bottom", 5,
				NULL);
				if (wired_state ()) {
					gtk_switch_set_active (wired_switch, FALSE);
					gtk_widget_hide (wired_dropdown_menu);
				} else {
					gtk_switch_set_active (wired_switch, TRUE);
					gtk_switch_set_state (wired_switch, TRUE);
					gtk_widget_show (wired_dropdown_menu);
					g_object_set (wired_dropdown_menu,
					"margin-start",  100,
					"margin-top",    5,
					"margin-end",    100,
					"margin-bottom", 5,
					NULL );
				}
				if (wireless_state ()) {
					gtk_switch_set_active (wireless_switch, FALSE);
					gtk_widget_hide (dropdown_menu);
				} else {
					gtk_switch_set_active (wireless_switch, TRUE);
					gtk_switch_set_state (wireless_switch, TRUE);
					gtk_widget_show (dropdown_menu);
					g_object_set (dropdown_menu,
						"margin-start",  100,
						"margin-top",    5,
						"margin-end",    100,
						"margin-bottom", 5,
						NULL );
				}
				GtkWidget *wired_label =
				gtk_label_new("Wired Connection");
				gtk_widget_set_valign (wired_label, GTK_ALIGN_CENTER);
				gtk_widget_set_halign (wired_label, GTK_ALIGN_CENTER);
				g_object_set (wired_label,
					"margin-start",  50,
					"margin-top",    10,
					"margin-end",    50,
					"margin-bottom", 5,
					NULL );
				gtk_box_append (GTK_BOX (wired_box), wired_label);
				gtk_box_append (GTK_BOX (wired_box), wired_switch);
				gtk_box_append (GTK_BOX (network_box), wired_box);
				gtk_box_append (GTK_BOX (network_box),
					wired_dropdown_menu);
				gtk_widget_set_valign (wired_box, GTK_ALIGN_CENTER);
				gtk_widget_set_halign (wired_box, GTK_ALIGN_CENTER);
				GtkWidget *wireless_label =
				gtk_label_new("Wireless Connection");
				gtk_widget_set_valign(wireless_label, GTK_ALIGN_CENTER);
				gtk_widget_set_halign(wireless_label, GTK_ALIGN_CENTER);
				g_object_set (wireless_label,
					"margin-start",  50,
					"margin-top",    5,
					"margin-end",    50,
					"margin-bottom", 5,
					NULL );
				gtk_box_append(GTK_BOX(wireless_box), wireless_label);
				gtk_box_append(GTK_BOX(wireless_box), wireless_switch);
				gtk_box_append(GTK_BOX(network_box), wireless_box);
				gtk_box_append(GTK_BOX(network_box), dropdown_menu);
				gtk_widget_set_valign(wireless_box, GTK_ALIGN_CENTER);
				gtk_widget_set_halign(wireless_box, GTK_ALIGN_CENTER);
				gtk_stack_add_named(GTK_STACK(stack), network_box, c);
				g_object_set(gtk_stack_get_page(GTK_STACK(stack),
						network_box), "title", c, NULL);
			} else if (i == 5) {
				//GtkWidget *icon_view;
				//gtk_box_append (GTK_BOX (vbox), icon_view);
				//GtkListStore *store;
				//GtkCellRenderer *renderer;
				//store = create_store();
				//display_settings_monitors(store);
				//icon_view = gtk_icon_view_new_with_model (GTK_TREE_MODEL (store));
				//g_object_unref (store);
				//gtk_icon_view_set_selection_mode (GTK_ICON_VIEW (icon_view),
				                                  //GTK_SELECTION_SINGLE);
				//gtk_icon_view_set_item_orientation (GTK_ICON_VIEW (icon_view),
				                                    //GTK_ORIENTATION_HORIZONTAL);
				//gtk_icon_view_set_columns (GTK_ICON_VIEW (icon_view), 1);
				//gtk_icon_view_set_reorderable (GTK_ICON_VIEW (icon_view), TRUE);

				//renderer = gtk_cell_renderer_pixbuf_new ();
				//gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (icon_view),
				                            //renderer, TRUE);
				//gtk_cell_layout_set_cell_data_func (GTK_CELL_LAYOUT (icon_view),
				                                    //renderer,
				                                    //set_cell_color,
				                                    //NULL, NULL);
				//renderer = gtk_cell_renderer_text_new ();
				//gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (icon_view),
				 //                           renderer, TRUE);
				//g_object_set (renderer, "editable", TRUE, NULL);
				//g_signal_connect (renderer, "edited", G_CALLBACK (edited), icon_view);
				//gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (icon_view),
                                //      renderer,
                                //      "text", COL_TEXT,
                                //      NULL);
				//gtk_box_append (GTK_BOX (vbox), icon_view);
				GtkWidget *dframe;
				GtkWidget *hpaned;
				GtkWidget *vpaned;
				GtkWidget *dlabel;
				GtkWidget *vbox;
				vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 8);
				gtk_widget_set_margin_start (vbox, 8);
				gtk_widget_set_margin_end (vbox, 8);
				gtk_widget_set_margin_top (vbox, 8);
				gtk_widget_set_margin_bottom (vbox, 8);
				dframe = gtk_frame_new (NULL);
				gtk_box_append (GTK_BOX (vbox), dframe);
				vpaned = gtk_paned_new (GTK_ORIENTATION_VERTICAL);
				gtk_frame_set_child (GTK_FRAME (dframe), vpaned);
				hpaned = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
				gtk_paned_set_start_child (GTK_PANED (vpaned), hpaned);
				gtk_paned_set_shrink_start_child (GTK_PANED (vpaned), TRUE);
				char monitor[2048];
				FILE *process = popen("xrandr --listmonitors | tail -n +2", "r");
				while (fgets(monitor, sizeof(monitor), process) != NULL) {
					char *p = strchr(monitor, '\n');
					if (p) *p = '\0';
					dlabel = gtk_button_new_with_label (monitor);
					gtk_widget_set_margin_start (dlabel, 4);
					gtk_widget_set_margin_end (dlabel, 4);
					gtk_widget_set_margin_top (dlabel, 4);
					gtk_widget_set_margin_bottom (dlabel, 4);
					gtk_widget_set_hexpand (dlabel, TRUE);
					gtk_widget_set_vexpand (dlabel, TRUE);
					gtk_paned_set_end_child (GTK_PANED (hpaned), dlabel);
					gtk_paned_set_shrink_end_child (GTK_PANED (hpaned), TRUE);
				}
				pclose(process);
				dlabel = gtk_label_new ("Goodbye");
				gtk_widget_set_margin_start (dlabel, 4);
				gtk_widget_set_margin_end (dlabel, 4);
				gtk_widget_set_margin_top (dlabel, 4);
				gtk_widget_set_margin_bottom (dlabel, 4);
				gtk_widget_set_hexpand (dlabel, TRUE);
				gtk_widget_set_vexpand (dlabel, TRUE);
				gtk_paned_set_end_child (GTK_PANED (vpaned), dlabel);
				gtk_paned_set_shrink_end_child (GTK_PANED (vpaned), FALSE);

				gtk_stack_add_named(GTK_STACK(stack), vbox, c);
				g_object_set (gtk_stack_get_page(GTK_STACK(stack), vbox),
						"title", c, NULL);
			} else if (i == 6) {
				GtkWidget *audio_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
				GtkWidget *output_box = output_devices_list();
				GtkWidget *input_box = input_devices_list();
				gtk_box_append(GTK_BOX(audio_box), output_box);
				gtk_box_append(GTK_BOX(audio_box), input_box);
				gtk_stack_add_named(GTK_STACK(stack), audio_box, c);
				g_object_set(gtk_stack_get_page(GTK_STACK(stack),
						audio_box), "title", c, NULL);
			} else if (i == 7) {
				GtkWidget *timezone_box =
				gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
				GtkWidget *timezone_box2 = select_timezone ();
				GtkWidget *save_button =
					gtk_button_new_with_label("Save");
				g_signal_connect(G_OBJECT(save_button),
					"clicked",
					G_CALLBACK(timezone_save_button_clicked)
					, NULL);
				g_object_set (save_button,
					"margin-start",  100,
					"margin-top",    600,
					"margin-end",    100,
					"margin-bottom", 5,
					NULL );
				gtk_widget_set_margin_bottom (save_button, 5);
				gtk_box_append(GTK_BOX(timezone_box), timezone_box2);
				gtk_box_append(GTK_BOX(timezone_box), save_button);
				gtk_stack_add_named(GTK_STACK(stack), timezone_box, c);
				g_object_set(gtk_stack_get_page(GTK_STACK(stack),
					timezone_box), "title", c, NULL);
			} else if (i == 8) {
				GtkWidget *apps_widget;
				if (!apps_window) {
					GtkWidget *list, *sw;
					GListModel *model;
					GtkListItemFactory *factory;
					gtk_window_set_default_size(GTK_WINDOW(
							apps_window),640, 320);
					gtk_window_set_display (GTK_WINDOW (apps_window),
                                          gtk_widget_get_display (apps_widget));
					gtk_window_set_title(GTK_WINDOW(apps_window),
					     "Applications");
					g_object_add_weak_pointer(G_OBJECT(apps_window),
						  (gpointer *) &apps_window);
					factory = gtk_signal_list_item_factory_new ();
					g_signal_connect(factory, "setup",
					 G_CALLBACK(setup_listitem_cb), NULL);
					g_signal_connect(factory, "bind",
					 G_CALLBACK(bind_listitem_cb), NULL);
					model = create_application_list ();
					list = gtk_list_view_new(GTK_SELECTION_MODEL(gtk_single_selection_new(model)),factory);
					g_signal_connect(list, "activate",
					 G_CALLBACK(activate_cb), NULL);
					sw = gtk_scrolled_window_new();
					gtk_window_set_child(GTK_WINDOW (apps_window), sw);
					gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sw),
						      list);
					gtk_stack_add_named(GTK_STACK(stack), sw, c);
					g_object_set (gtk_stack_get_page(GTK_STACK(stack), sw),
						"title", c, NULL);
				}
			} else if (i == 9) {
				widget = password_for_updates();
				gtk_stack_add_named(GTK_STACK(stack), widget, c);
				g_object_set(gtk_stack_get_page(GTK_STACK(stack), widget), "title", c, NULL);
			} else {
				widget = gtk_label_new (c);
				gtk_stack_add_named(GTK_STACK(stack), widget, c);
				g_object_set(gtk_stack_get_page(GTK_STACK(stack),widget), "title", c, NULL);
			}
		}
		gtk_window_set_child (GTK_WINDOW (window), box);
	}
	if (!gtk_widget_get_visible (window))
		gtk_widget_show (window);
	else
		gtk_window_destroy (GTK_WINDOW (window));
}

int
main (int argc, char **argv)
{
	GtkApplication *app;
	int status;
	app = gtk_application_new("org.chaos.settings_menu", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);
	return status;
}
