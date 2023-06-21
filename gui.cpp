#include "gui.h"

//#include <cairomm/context.h>
#include <iostream>

MainWindow::MainWindow()
: start_button("Start"),force_label("0.0"),
  m_Dispatcher(),  m_Worker(),m_WorkerThread(nullptr) 
{
  // Sets the border width of the window.
  set_border_width(10);
  set_default_size(1100,900);
  //start_button.set_default_size(50,20);
  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  start_button.signal_clicked().connect(sigc::mem_fun(*this,
              &MainWindow::on_start_clicked));
 // stop_button.signal_clicked().connect(sigc::mem_fun(*this,
   //           &MainWindow::on_stop_clicked));
  m_Dispatcher.connect(sigc::mem_fun(*this, 
              &MainWindow::on_notification_from_worker_thread));

  // This packs the button into the Window (a container).
  m_grid.set_row_homogeneous(true);
  m_grid.set_column_homogeneous(true);
  add(m_grid);
  m_grid.attach(area,2, 0, 10, 40);
  m_grid.attach(start_button,0, 0, 1, 2);
  m_grid.attach(force_label,0, 4, 1, 2);
  
  //m_box1.pack_start(start_button);
 // m_box1.pack_start(force_label);
 // m_box1.pack_start(stop_button);
  //add(start_button);
  //add(force_label);
 // add(stop_button);

  // The final step is to display this newly created widget...
  start_button.show();
  force_label.show();
 // stop_button.show();
 // m_grid.pack_start(area);
//  update_start_stop_buttons();
  area.show();
  m_grid.show();
}
  
   

MainWindow::~MainWindow(){}

void MainWindow::on_start_clicked()
{ if(start_button.get_label() == "Start"){
    std::cout << "Hello World" << std::endl;
    if (m_WorkerThread)
      {
        std::cout << "Can't start a worker thread while another one is running." << std::endl;
    }
    else
    {
    // Start a new worker thread.
      start_button.set_label("Stop");
      m_WorkerThread = new std::thread(
            [this]
                  {
                    m_Worker.do_work(this);
                  });
    }
  }
  else{
    std::cout << "bye World" << std::endl;
    start_button.set_label("Start");
    if (!m_WorkerThread){
      std::cout << "Can't stop a worker thread. None is running." << std::endl;
    }
    else{
   // Order the worker thread to stop.
    m_Worker.stop_work();
    if (m_WorkerThread->joinable())
      m_WorkerThread->join();
   // stop_button.set_sensitive(false);
    }
     
  }
 // update_start_stop_buttons();
}
/*
void MainWindow::on_stop_clicked()
{
  std::cout << "bye World" << std::endl;
  if (!m_WorkerThread)
  {
    std::cout << "Can't stop a worker thread. None is running." << std::endl;
  }
  else
  {
   // Order the worker thread to stop.
    m_Worker.stop_work();
    if (m_WorkerThread->joinable())
      m_WorkerThread->join();
    stop_button.set_sensitive(false);
  }
  
 // hide();
}
void MainWindow::update_start_stop_buttons()
{
  const bool thread_is_running = m_WorkerThread != nullptr;

  start_button.set_sensitive(!thread_is_running);
  stop_button.set_sensitive(thread_is_running);
}*/
void MainWindow::update_widgets()
{
  ForceData force_data{};
  Glib::ustring message_from_worker_thread;
  m_Worker.get_data(&force_data, &message_from_worker_thread);
 // std::cout << fraction_done<< std::endl;
  area.data.push_back(force_data);
  area.queue_draw();
  
  //m_ProgressBar.set_fraction(fraction_done);
/*
  if (message_from_worker_thread != m_TextView.get_buffer()->get_text())
  {
    auto buffer = m_TextView.get_buffer();
    buffer->set_text(message_from_worker_thread);

    // Scroll the last inserted line into view. That's somewhat complicated.
    Gtk::TextIter iter = buffer->end();
    iter.set_line_offset(0); // Beginning of last line
    auto mark = buffer->get_mark("last_line");
    buffer->move_mark(mark, iter);
    m_TextView.scroll_to(mark);
    // TextView::scroll_to(iter) is not perfect.
    // We do need a TextMark to always get the last line into view.
  }*/
}
void MainWindow::notify()
{
  m_Dispatcher.emit();
}
void MainWindow::on_notification_from_worker_thread()
{
  if (m_WorkerThread && m_Worker.has_stopped())
  {
    // Work is done.
    if (m_WorkerThread->joinable())
      m_WorkerThread->join();
    delete m_WorkerThread;
    m_WorkerThread = nullptr;
  // update_start_stop_buttons();
  }
  update_widgets();
}


MyArea::MyArea()
{
}

MyArea::~MyArea()
{
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();
 // std::cout << y << std::endl;
  // coordinates for the center of the window
  

  cr->set_line_width(0.2);

  // draw red lines out from the center of the window
  cr->set_source_rgb(0.8, 0.0, 0.0);
 // cr->move_to(0, 100);

 for(ForceData point:data){
   //below line should put out off loop
   
   axe.update(width, height,data.size(),point.max_force/*, 
                point.min_force*/);
  
    std::cout << point.max_force << "----" << endl;
    //std::cout <<point.sample_number<< '-' << point.force << std::endl;
   // std::cout << axe.x(point.sample_number)<<'-'<< axe.y(point.force);
   // std::cout << std::endl;
    cr->line_to(axe.x(point.sample_number), axe.y(point.force));
    cr->move_to(axe.x(point.sample_number), axe.y(point.force));
    
    auto layout = create_pango_layout("Hi there!"); 
    layout->show_in_cairo_context(cr);  
 
 }
  cr->stroke();
  axe.draw(cr);
  
 // Pango::FontDescription font;
//  font.set_family("Monospace");
 // font.set_weight(Pango::WEIGHT_BOLD);
  //auto layout = create_pango_layout("Hi there!");
 // layout->set_font_description(font);

 // int text_width;
  //int text_height;
  //layout->get_pixel_size(text_width, text_height);

  // Position the text in the middle
 // cr->move_to((rectangle_width-text_width)/2, (rectangle_height-text_height)/2);

 // layout->show_in_cairo_context(cr);
  

  return true;
}

