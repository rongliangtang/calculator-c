#include <stdio.h>
#include <string.h>
#include "console_ui.h"
#include "ut_ui.h"
#include "gtk_ui.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (strcasecmp(argv[1], "console") == 0) {
      return RunConsoleUi(argc, argv);
    } else if (strcasecmp(argv[1], "ut") == 0) {
      return RunUtUi(argc, argv);
    }
  }
  RunGtkUi(argc, argv);

  return 0;
}
