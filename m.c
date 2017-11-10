#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "cJSON.h"

int main(int argc, char **argv){
  char *json_file_contents;
  long json_file_size;
  FILE *json_file = fopen("/home/jason/m.cfg", "rb");

  fseek(json_file, 0, SEEK_END);
  json_file_size = ftell(json_file);
  rewind(json_file);
  json_file_contents = malloc(json_file_size * (sizeof(char)));
  fread(json_file_contents, sizeof(char), json_file_size, json_file);
  fclose(json_file);

  char arr_menu_keys[100][100];
  char arr_menu_commands[100][1000];
  char user_menu_selection[1];
  int command_count = 0;
  int commandline_arg;
  char *menu_section_to_display = "topmenuitems";
  char command_arg_selection[1] = {-1};
  static struct option long_options[] = {
    {"menuname", required_argument, 0, 1001 },
  };
  int long_index = 0;

  while ((commandline_arg = getopt_long_only(argc, argv,
                                         "abcdefghijklmnogqrstuvwxyzABCDEFGHIJKLMNOGQRSTUVWXYZ1234567890",
                                         long_options, &long_index )) != -1){
    switch (commandline_arg) {
    case 1001 :
      // --menuname
      if (optarg != NULL){
        menu_section_to_display = optarg;
      }
      break;
    default:
      // -letter
      // e.g. m -g (starts gedit)
      command_arg_selection[0] = commandline_arg;
      break;
    }
  }
  cJSON *json_root = cJSON_Parse(json_file_contents);
  cJSON *json_menu = cJSON_GetObjectItem(json_root, menu_section_to_display);
  int menu_item_count = cJSON_GetArraySize(json_menu);
  printf("########################\n");
  for (int i = 0; i < menu_item_count; i++) {
    cJSON *json_menu_item = cJSON_GetArrayItem(json_menu, i);
    char *title = cJSON_GetObjectItem(json_menu_item, "title")->valuestring;
    char *key = cJSON_GetObjectItem(json_menu_item, "key")->valuestring;
    char *command = cJSON_GetObjectItem(json_menu_item, "command")->valuestring;
    strcpy(arr_menu_keys[i+1], key);
    strcpy(arr_menu_commands[i+1], command);
    // show each menu item on the screen
    printf("(%s %s\n", arr_menu_keys[i+1], title);
    command_count++;
  }
  printf("########################\n");
  putchar('\n');
  fflush(stdout);
  if (command_arg_selection[0] != -1){
    user_menu_selection[0] = command_arg_selection[0];
  }else{
    // ask for a letter from the menu (Ctrl-C and Enter allowed to cancel)
    scanf("%2000[^\n]", user_menu_selection);
    fflush(stdout);
  }

  for(int j = 1; j < command_count+1; ++j){
    int is_selection = strcmp(arr_menu_keys[j], user_menu_selection);
    if (is_selection == 0){
      // run the system command assosiated with the menu item chosen
      //printf("Running: %s\n", arr_menu_commands[j]);
      system(arr_menu_commands[j]);
      break;
    }
  }

  cJSON_Delete(json_root);
  return 0;
}
