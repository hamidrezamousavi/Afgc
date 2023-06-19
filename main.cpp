#include "lib.h"
#include <future> 
#include <iostream>
#include "gui.h"
#include <gtkmm/application.h>


int main (int argc, char *argv[])
{
  
 auto app = Gtk::Application::create(argc, argv, "AFGC Reader");

  MainWindow mainWindow;

  //Shows the window and returns when it is closed.
  return app->run(mainWindow);
 
}
