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
keybindings (void)
{
  GtkWidget *keybindings_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *keybindings_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 100);
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
  for (int idx = 0; idx < size; idx++)
  {
    GtkWidget* separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append (GTK_BOX (keybindings_vbox), separator);
    GtkWidget *keybinding_pair_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 100);
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
    gtk_box_append ( GTK_BOX (keybindings_vbox), keybinding_pair_hbox);
  }
  return keybindings_vbox;
}

/*
 *
 * Tint2 Configurations
 *
 */ 

//GtkWidget *window;

static char*
hex_to_rgb (char *hex);

static char*
rgb_to_hex (char *rgb);

static char*
tint2_panel_color (void)
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

GtkWidget*
tint2_configuration ()
{
  GtkWidget *tint2_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *panel_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
  GtkWidget *panel_label = gtk_label_new ("Panel Color: ");
  GtkColorDialog *panel_color_dialog = gtk_color_dialog_new ();
  GtkWidget* panel_color_dialog_button = gtk_color_dialog_button_new (panel_color_dialog);
  gtk_box_append (GTK_BOX (panel_hbox), panel_label);
  gtk_box_append (GTK_BOX (panel_hbox), panel_color_dialog_button);
  gtk_widget_set_halign (panel_label, GTK_ALIGN_CENTER);
  gtk_widget_set_halign (panel_color_dialog_button, GTK_ALIGN_CENTER);
  g_object_set (panel_color_dialog_button,
      "margin-start",  60,
      "margin-top",    10,
      //"margin-end",    5,
      "margin-bottom", 10,
      NULL );
  gtk_box_append (GTK_BOX (tint2_vbox), panel_hbox);
  gtk_widget_set_halign (tint2_vbox, GTK_ALIGN_CENTER);
  g_object_set (tint2_vbox,
      "margin-start",  50,
      "margin-top",    10,
      //"margin-end",    50,
      "margin-bottom", 10,
      NULL );
  //char *panel_custom_color_hex = tint2_panel_color ();
  //g_print (panel_custom_color_hex);
  //gtk_color_dialog_set_with_alpha (panel_color_dialog, TRUE);
  GdkRGBA *panel_custom_color_rgba;
  gdk_rgba_parse (panel_custom_color_rgba, "\\#aa7733");
  //gdk_rgba_parse (panel_custom_color_rgba, panel_custom_color_hex);
  //gtk_color_dialog_button_set_rgba (panel_color_dialog_button, panel_custom_color_rgba);
  //gtk_color_dialog_button_set_rgba (panel_color_dialog_button, panel_custom_color_rgba);
  GtkWidget *workstation_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
  GtkWidget *workstation_label = gtk_label_new ("Workstation Color: ");
  GtkColorDialog *workstation_color_dialog = gtk_color_dialog_new ();
  GtkWidget* workstation_color_dialog_button = gtk_color_dialog_button_new (workstation_color_dialog);
  gtk_box_append (GTK_BOX (workstation_hbox), workstation_label);
  gtk_box_append (GTK_BOX (workstation_hbox), workstation_color_dialog_button);
  gtk_widget_set_halign (workstation_label, GTK_ALIGN_CENTER);
  gtk_widget_set_halign (workstation_color_dialog_button, GTK_ALIGN_CENTER);
  g_object_set (workstation_color_dialog_button,
      "margin-start",  5,
      "margin-top",    10,
      //"margin-end",    5,
      "margin-bottom", 10,
      NULL );
  gtk_box_append (GTK_BOX (tint2_vbox), workstation_hbox);
  gtk_widget_set_halign (tint2_vbox, GTK_ALIGN_CENTER);
  g_object_set (tint2_vbox,
      "margin-start",  5,
      "margin-top",    10,
      //"margin-end",    50,
      "margin-bottom", 10,
      NULL );
  //char *workstation_custom_color_hex = tint2_workstation_color ();
  //g_print (workstation_custom_color_hex);
  //gtk_color_dialog_set_with_alpha (workstation_color_dialog, TRUE);
  GdkRGBA *workstation_custom_color_rgba;
  gdk_rgba_parse (workstation_custom_color_rgba, "\\#aa7733");
  GtkWidget *status_bar_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 50);
  GtkWidget *status_bar_label = gtk_label_new ("Status Bar Color: ");
  GtkColorDialog *status_bar_color_dialog = gtk_color_dialog_new ();
  GtkWidget* status_bar_color_dialog_button = gtk_color_dialog_button_new (status_bar_color_dialog);
  gtk_box_append (GTK_BOX (status_bar_hbox), status_bar_label);
  gtk_box_append (GTK_BOX (status_bar_hbox), status_bar_color_dialog_button);
  gtk_widget_set_halign (status_bar_label, GTK_ALIGN_CENTER);
  gtk_widget_set_halign (status_bar_color_dialog_button, GTK_ALIGN_CENTER);
  g_object_set (status_bar_color_dialog_button,
      "margin-start",  5,
      "margin-top",    10,
      //"margin-end",    5,
      "margin-bottom", 10,
      NULL );
  gtk_box_append (GTK_BOX (tint2_vbox), status_bar_hbox);
  gtk_widget_set_halign (tint2_vbox, GTK_ALIGN_CENTER);
  g_object_set (tint2_vbox,
      "margin-start",  5,
      "margin-top",    10,
      //"margin-end",    50,
      "margin-bottom", 10,
      NULL );
  //char *status_bar_custom_color_hex = tint2_status_bar_color ();
  //g_print (status_bar_custom_color_hex);
  //gtk_color_dialog_set_with_alpha (status_bar_color_dialog, TRUE);
  GdkRGBA *status_bar_custom_color_rgba;
  gdk_rgba_parse (status_bar_custom_color_rgba, "\\#aa7733");
  GtkWidget *window_switcher_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
  GtkWidget *window_switcher_label = gtk_label_new ("Window Switcher Color: ");
  GtkColorDialog *window_switcher_color_dialog = gtk_color_dialog_new ();
  GtkWidget* window_switcher_color_dialog_button = gtk_color_dialog_button_new (window_switcher_color_dialog);
  gtk_box_append (GTK_BOX (window_switcher_hbox), window_switcher_label);
  gtk_box_append (GTK_BOX (window_switcher_hbox), window_switcher_color_dialog_button);
  gtk_widget_set_halign (window_switcher_label, GTK_ALIGN_CENTER);
  gtk_widget_set_halign (window_switcher_color_dialog_button, GTK_ALIGN_CENTER);
  g_object_set (window_switcher_color_dialog_button,
      //"margin-start",  5,
      "margin-top",    10,
      //"margin-end",    5,
      "margin-bottom", 10,
      NULL );
  gtk_box_append (GTK_BOX (tint2_vbox), window_switcher_hbox);
  gtk_widget_set_halign (tint2_vbox, GTK_ALIGN_CENTER);
  g_object_set (tint2_vbox,
      "margin-start",  5,
      "margin-top",    10,
      //"margin-end",    50,
      "margin-bottom", 10,
      NULL );
  //char *window_switcher_custom_color_hex = tint2_window_switcher_color ();
  //g_print (window_switcher_custom_color_hex);
  //gtk_color_dialog_set_with_alpha (window_switcher_color_dialog, TRUE);
  GdkRGBA *window_switcher_custom_color_rgba;
  gdk_rgba_parse (window_switcher_custom_color_rgba, "\\#00000000");
  GtkWidget* separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_box_append (GTK_BOX (tint2_vbox), separator);
  GtkWidget *view_port_test_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
  GtkWidget *view_port_test_label = gtk_label_new ("Window Switcher Color: ");
  GtkColorDialog *view_port_test_color_dialog = gtk_color_dialog_new ();
  GtkWidget* view_port_test_color_dialog_button = gtk_color_dialog_button_new (view_port_test_color_dialog);
  gtk_box_append (GTK_BOX (view_port_test_hbox), view_port_test_label);
  gtk_box_append (GTK_BOX (view_port_test_hbox), view_port_test_color_dialog_button);
  gtk_widget_set_halign (view_port_test_label, GTK_ALIGN_CENTER);
  gtk_widget_set_halign (view_port_test_color_dialog_button, GTK_ALIGN_CENTER);
  g_object_set (view_port_test_color_dialog_button,
      //"margin-start",  5,
      "margin-top",    10,
      //"margin-end",    5,
      "margin-bottom", 10,
      NULL );
  gtk_box_append (GTK_BOX (tint2_vbox), view_port_test_hbox);
  gtk_widget_set_halign (tint2_vbox, GTK_ALIGN_CENTER);
  g_object_set (tint2_vbox,
      "margin-start",  5,
      "margin-top",    10,
      //"margin-end",    50,
      "margin-bottom", 10,
      NULL );
  //char *view_port_test_custom_color_hex = tint2_view_port_test_color ();
  //g_print (view_port_test_custom_color_hex);
  //gtk_color_dialog_set_with_alpha (view_port_test_color_dialog, TRUE);
  GdkRGBA *view_port_test_custom_color_rgba;
  gdk_rgba_parse (view_port_test_custom_color_rgba, "\\#00000000");
  return tint2_vbox;
}

/*
 *
 * Network Settings
 *
 */

static char*
wired_interface (void)
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
  if (!sl)
    gtk_string_list_append (sl, "No wired connection");

  pclose(connection);
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
  if (state)
    {
      gtk_switch_set_state (switch_button, state);
      wired_string_list =  gtk_string_list_new ((const char * const *) wired_connections);
      wired_names (wired_string_list);
      wired_dropdown_menu = gtk_drop_down_new(wired_string_list, NULL);
      gtk_drop_down_set_enable_search (wired_dropdown_menu, TRUE);
      g_object_set (wired_dropdown_menu,
      "margin-start",  50,
      "margin-top",    10,
      "margin-end",    50,
      "margin-bottom", 50,
      NULL );
      gtk_widget_show (wired_dropdown_menu);
    }
  else
    {
      gtk_switch_set_state (switch_button, FALSE);
      gtk_switch_set_active (switch_button, FALSE);
      gtk_widget_hide (wired_dropdown_menu);
      wired_string_list =  gtk_string_list_new ((const char * const *) wired_connections);
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
  if (state)
    {
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
    }
  else
    {
      system("nmcli radio wifi off");
      gtk_switch_set_state (switch_button, FALSE);
      gtk_switch_set_active (switch_button, FALSE);
      gtk_widget_hide (dropdown_menu);
      string_list =  gtk_string_list_new ((const char * const *) connections);
      dropdown_menu = gtk_drop_down_new(string_list, NULL);
    }
  return TRUE;
}

/*
 *
 *  Display Settings
 *
 */

//TODO

/*
 *
 * Volume & Audio Settings
 *
 */

static int
output_devices_list_size (void)
{
  int count;
  char buffer[2048];
  FILE *command = popen("pamixer --list-sinks | tail -n +2 | wc -l", "r");
  while (fgets (buffer, sizeof (buffer), command) != NULL) {
        char *p = strchr (buffer, '\n');
        if (p) *p = '\0'; 
    }
  pclose(command);
  sscanf(buffer, "%d", &count);
  return count;
}


const char *output_devices[] = {NULL};
GtkStringList *output_devices_string_list;
static GtkWidget*
output_devices_list (void)
{
  GtkWidget *output_devices_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_set_valign (output_devices_box, GTK_ALIGN_START);
  GtkWidget *output_devices_label = gtk_label_new ("Output Devices");
  gtk_widget_set_valign (output_devices_label, GTK_ALIGN_START);
  gtk_widget_set_halign (output_devices_label, GTK_ALIGN_START);
  gtk_box_append (GTK_BOX (output_devices_box), output_devices_label);
  //pamixer --list-sinks | tail -n +2 | awk '{ print substr($0, index($0,$4)) }' | tr -d '"'
  char buffer[2048];
  gtk_box_set_homogeneous (GTK_BOX (output_devices_box), TRUE);
  output_devices_string_list =  gtk_string_list_new ((const char * const *) output_devices);
  FILE *command = popen ("pamixer --list-sinks | tail -n +2 | awk '{ print substr($0, index($0,$4)) }' | tr -d '\"'", "r");
  //if (!command)
  //    return NULL;
  int size = output_devices_list_size ();
  while (fgets (buffer, sizeof (buffer), command) != NULL) {
      char *p = strchr (buffer, '\n');
      if (p) *p = '\0'; 
      gtk_string_list_append (output_devices_string_list, buffer);
  }
  pclose(command);
  for (int idx = 0; idx < size; idx++)
  {
      GtkWidget *output_device_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
      //gtk_widget_set_valign (output_device_vbox, GTK_ALIGN_START);

      GtkWidget *output_device_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 60);
      //gtk_widget_set_halign (output_device_hbox, GTK_ALIGN_CENTER);

      GtkWidget *output_device_name = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
      //gtk_widget_set_halign (output_device_name, GTK_ALIGN_START);

      GtkWidget *output_device_features = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
      //gtk_widget_set_halign (output_device_features, GTK_ALIGN_END);

      char *output_device = gtk_string_list_get_string (output_devices_string_list, idx);

      GtkWidget *output_device_label = gtk_label_new (output_device);

      GtkWidget *output_device_mute_button = gtk_button_new_with_label ("Mute");
      GtkWidget *output_device_default_button = gtk_button_new_with_label ("Default");

      gtk_box_append (GTK_BOX (output_device_name), output_device_label);
      gtk_widget_set_halign (output_device_name, GTK_ALIGN_START);

      gtk_box_append (GTK_BOX (output_device_features), output_device_mute_button);

      gtk_box_append (GTK_BOX (output_device_features), output_device_default_button);
      gtk_widget_set_halign (output_device_features, GTK_ALIGN_END);

      gtk_box_append (GTK_BOX (output_device_hbox), output_device_name);

      gtk_box_append (GTK_BOX (output_device_hbox), output_device_features);
      gtk_widget_set_halign (output_device_hbox, GTK_ALIGN_CENTER);

      //gtk_box_append (GTK_BOX (output_device_hbox), output_device_default_button);
      GtkWidget *output_device_scale = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0.0, 153.0, 1.0);
      GtkWidget *separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
      //gtk_box_append (GTK_BOX (output_device_hbox), output_device_name);
      gtk_box_append (GTK_BOX (output_device_vbox), output_device_hbox);
      gtk_box_append (GTK_BOX (output_device_vbox), output_device_scale);
      gtk_box_append (GTK_BOX (output_device_vbox), separator);
      gtk_widget_set_valign (output_device_vbox, GTK_ALIGN_START);
      gtk_box_append (GTK_BOX (output_devices_box), output_device_vbox);
  }
  return output_devices_box;
}

/*
 *
 *
 * Time & Language Settings
 *
 */

static char*
current_timezone (void)
{
  char buffer[2048];
  FILE *command = popen ("readlink /etc/localtime | cut -d '/' -f5-", "r");
  while (fgets (buffer, sizeof (buffer), command) != NULL) {
      char *p = strchr (buffer, '\n');
      if (p) *p = '\0'; 
  }
  pclose(command);
  char *current_localtime = buffer;
  return current_localtime;
}

static int
current_timezone_position (void)
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
select_timezone (void)
{
    GtkWidget *timezone_box, *label;
    timezone_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 15);
    gtk_box_set_homogeneous (GTK_BOX (timezone_box), TRUE);
    label = gtk_label_new ("Select a timezone:");
    gtk_box_append (GTK_BOX (timezone_box), label);
    timezone_string_list =  gtk_string_list_new ((const char * const *) timezones);
    //FILE *command = popen ("find /usr/share/zoneinfo -print | cut -d '/' -f5- | sort | nl", "r");
    FILE *command = popen ("find /usr/share/zoneinfo/ -type f -exec grep -IL . \"{}\" \\; | cut -d '/' -f5- | sort | nl", "r");
    //if (!command)
    //    return NULL;
    char buffer[2048];
    int position = current_timezone_position ();
    while (fgets (buffer, sizeof (buffer), command) != NULL) {
        char *p = strchr (buffer, '\n');
        if (p) *p = '\0'; 
        gtk_string_list_append (timezone_string_list, buffer);
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
timezone_save_button_clicked (GtkWidget *widget, 
        gpointer data)
{
  guint selected_timezone_position = gtk_drop_down_get_selected (GTK_DROP_DOWN (timezone_dropdown_menu));
  int position = selected_timezone_position, idx = 0;
  char *timezone, *selected_timezone = gtk_string_list_get_string (timezone_string_list, position);
  while (selected_timezone[idx] != '\0')
  {
      if (selected_timezone[idx] <= 32 && selected_timezone[idx + 1] > 32)
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
                   GtkListItem        *list_item)
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
  image = gtk_widget_get_first_child (gtk_list_item_get_child (list_item));
  label = gtk_widget_get_next_sibling (image);
  app_info = gtk_list_item_get_item (list_item);
  gtk_image_set_from_gicon (GTK_IMAGE (image), g_app_info_get_icon (app_info));
  gtk_label_set_label (GTK_LABEL (label), g_app_info_get_display_name (app_info));
}

static void
activate_cb (GtkListView  *list,
             guint         position,
             gpointer      unused)
{
  GAppInfo *app_info;
  GdkAppLaunchContext *context;
  GError *error = NULL;
  app_info = g_list_model_get_item (G_LIST_MODEL (gtk_list_view_get_model (list)), position);
  context = gdk_display_get_app_launch_context (gtk_widget_get_display (GTK_WIDGET (list)));
  if (!g_app_info_launch (app_info,
                          NULL,
                          G_APP_LAUNCH_CONTEXT (context),
                          &error))
    {
      GtkWidget *dialog;
      dialog = gtk_message_dialog_new (GTK_WINDOW (gtk_widget_get_root (GTK_WIDGET (list))),
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

  if (!window)
    {
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
      for (i=0; (c = *(pages+i)) != NULL; i++ )
         {
           if (i == 0)
             {
               main_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 25);
               gtk_widget_set_valign (main_box, GTK_ALIGN_CENTER);
               gtk_widget_set_halign (main_box, GTK_ALIGN_CENTER);
               widget = gtk_image_new_from_file ("we_are_fsociety.png");
               gtk_widget_add_css_class (widget, "icon-dropshadow");
               gtk_image_set_pixel_size (GTK_IMAGE (widget), 500);
               gtk_box_append (GTK_BOX (main_box), widget);
               main_label = gtk_label_new ("chaOS - Settings & Usage Guide");
               gtk_box_append (GTK_BOX (main_box), main_label);
               gtk_stack_add_named (GTK_STACK (stack), main_box, c);
               g_object_set (gtk_stack_get_page (GTK_STACK (stack), main_box), "title", c, NULL);
             }
          else if (i == 1)
            {
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
               gtk_stack_add_named (GTK_STACK (stack), bindings_vbox, c);
               g_object_set (gtk_stack_get_page (GTK_STACK (stack), bindings_vbox), "title", c, NULL);
            }
          else if (i == 2)
            {
              //GtkColorDialog *color_dialog = gtk_color_dialog_new ();
              //GtkWidget *color_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
              //GtkWidget* color_dialog_button = gtk_color_dialog_button_new (color_dialog);
              //gtk_box_append (GTK_BOX (color_box), color_dialog_button);


              //const GdkRGBA* rgba = gtk_color_dialog_button_get_rgba (color_dialog_button);

              //char *color_result = gdk_rgba_to_string (color_dialog_button);

              //g_print(color_result);
              GtkWidget *tint2_vbox = tint2_configuration ();



              gtk_stack_add_named (GTK_STACK (stack), tint2_vbox, c);
              g_object_set (gtk_stack_get_page (GTK_STACK (stack), tint2_vbox), "title", c, NULL);
            }
          else if (i == 3)
            {
              widget = gtk_label_new (c);
              gtk_stack_add_named (GTK_STACK (stack), widget, c);
              g_object_set (gtk_stack_get_page (GTK_STACK (stack), widget), "title", c, NULL);
            }
          else if (i == 4)
            {
              GtkWidget *network_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
              GtkWidget *wired_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 35); 
              g_object_set (wired_box,
                "margin-start",  50,
                "margin-top",    30,
                "margin-end",    50,
                "margin-bottom", 10,
                NULL);
              GtkWidget *wireless_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 25); 
              GtkWidget *wired_switch = gtk_switch_new ();
              GtkWidget *wireless_switch = gtk_switch_new ();
              g_signal_connect(wired_switch, "state-set", G_CALLBACK (set_wired_switch_state), gtk_switch_get_active (wired_switch));
              g_signal_connect(wireless_switch, "state-set", G_CALLBACK (set_wireless_switch_state), gtk_switch_get_active (wireless_switch));
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
              if (wired_state ())
                {
                  gtk_switch_set_active (wired_switch, FALSE);
                  gtk_widget_hide (wired_dropdown_menu);
                }
              else
                {
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
              if (wireless_state ())
                {
                  gtk_switch_set_active (wireless_switch, FALSE);
                  gtk_widget_hide (dropdown_menu);
                }
              else
                {
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
              GtkWidget *wired_label = gtk_label_new ("Wired Connection");
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
              gtk_box_append (GTK_BOX (network_box), wired_dropdown_menu);
              gtk_widget_set_valign (wired_box, GTK_ALIGN_CENTER);
              gtk_widget_set_halign (wired_box, GTK_ALIGN_CENTER);
              GtkWidget *wireless_label = gtk_label_new ("Wireless Connection");
              gtk_widget_set_valign (wireless_label, GTK_ALIGN_CENTER);
              gtk_widget_set_halign (wireless_label, GTK_ALIGN_CENTER);
              g_object_set (wireless_label,
               "margin-start",  50,
               "margin-top",    5,
               "margin-end",    50,
               "margin-bottom", 5,
               NULL );
              gtk_box_append ( GTK_BOX (wireless_box), wireless_label);
              gtk_box_append ( GTK_BOX (wireless_box), wireless_switch);              
              gtk_box_append ( GTK_BOX (network_box), wireless_box);
              gtk_box_append ( GTK_BOX (network_box), dropdown_menu);              
              gtk_widget_set_valign (wireless_box, GTK_ALIGN_CENTER);
              gtk_widget_set_halign (wireless_box, GTK_ALIGN_CENTER);
              gtk_stack_add_named ( GTK_STACK (stack), network_box, c);
              g_object_set ( gtk_stack_get_page (GTK_STACK (stack), network_box), "title", c, NULL);
            }
          else if (i == 5)
            {
              widget = gtk_label_new (c);
              gtk_stack_add_named (GTK_STACK (stack), widget, c);
              g_object_set (gtk_stack_get_page (GTK_STACK (stack), widget), "title", c, NULL);
            }
          else if (i == 6)
            {
              GtkWidget *audio_box = output_devices_list (); 

              gtk_stack_add_named (GTK_STACK (stack), audio_box, c);
              g_object_set (gtk_stack_get_page (GTK_STACK (stack), audio_box), "title", c, NULL);
              
            }
          else if (i == 7)
            {
              GtkWidget *timezone_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
              GtkWidget *timezone_box2 = select_timezone ();
              GtkWidget *save_button = gtk_button_new_with_label ("Save");
              g_signal_connect(G_OBJECT(save_button), "clicked", G_CALLBACK(timezone_save_button_clicked), NULL);
              g_object_set (save_button,
                "margin-start",  100,
                "margin-top",    600,
                "margin-end",    100,
                "margin-bottom", 5,
                NULL );
              gtk_widget_set_margin_bottom (save_button, 5);
              gtk_box_append (GTK_BOX (timezone_box), timezone_box2);
              gtk_box_append (GTK_BOX (timezone_box), save_button);
              gtk_stack_add_named (GTK_STACK (stack), timezone_box, c);
              g_object_set (gtk_stack_get_page (GTK_STACK (stack), timezone_box), "title", c, NULL);
            }
          else if (i == 8)
            {
              GtkWidget *apps_widget;
              if (!apps_window)
                {
                  GtkWidget *list, *sw;
                  GListModel *model;
                  GtkListItemFactory *factory;
                  gtk_window_set_default_size (GTK_WINDOW (apps_window), 640, 320);
                  gtk_window_set_display (GTK_WINDOW (apps_window),
                                          gtk_widget_get_display (apps_widget));
                  gtk_window_set_title (GTK_WINDOW (apps_window), "Applications");
                  g_object_add_weak_pointer (G_OBJECT (apps_window), (gpointer *) &apps_window);
                  factory = gtk_signal_list_item_factory_new ();
                  g_signal_connect (factory, "setup", G_CALLBACK (setup_listitem_cb), NULL);
                  g_signal_connect (factory, "bind", G_CALLBACK (bind_listitem_cb), NULL);
                  model = create_application_list ();
                  list = gtk_list_view_new (GTK_SELECTION_MODEL (gtk_single_selection_new (model)), factory);
                  g_signal_connect (list, "activate", G_CALLBACK (activate_cb), NULL);
                  sw = gtk_scrolled_window_new ();
                  gtk_window_set_child (GTK_WINDOW (apps_window), sw);
                  gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (sw), list);
                  gtk_stack_add_named (GTK_STACK (stack), sw, c);
                  g_object_set (gtk_stack_get_page (GTK_STACK (stack), sw), "title", c, NULL);
                }
            }
          else if (i == 9)
            {
              widget = gtk_label_new (c);
              gtk_stack_add_named (GTK_STACK (stack), widget, c);
              g_object_set (gtk_stack_get_page (GTK_STACK (stack), widget), "title", c, NULL);
            }
          else
            {
                widget = gtk_label_new (c);
                gtk_stack_add_named (GTK_STACK (stack), widget, c);
                g_object_set (gtk_stack_get_page (GTK_STACK (stack), widget), "title", c, NULL);
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
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;
  app = gtk_application_new ("org.chaos.settings_menu", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  return status;
}
