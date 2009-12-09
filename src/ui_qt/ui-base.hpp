#define UNICODE
#define QT_NO_DEBUG
#define QT_CORE_LIB
#define QT_GUI_LIB
#define QT_THREAD_SUPPORT

#include <QApplication>
#include <QtGui>
//Q_IMPORT_PLUGIN(QJpegPlugin)
//Q_IMPORT_PLUGIN(QMngPlugin)

#include <../base.hpp>

#include <nall/base64.hpp>
#include <nall/config.hpp>
#include <nall/input.hpp>
#include <nall/ups.hpp>
using namespace nall;

#include <ruby/ruby.hpp>
using namespace ruby;

#include "config.hpp"
#include "interface.hpp"

#include "application/application.moc.hpp"

#include "base/about.moc.hpp"
#include "base/diskbrowser.moc.hpp"
#include "base/htmlviewer.moc.hpp"
#include "base/loader.moc.hpp"
#include "base/main.moc.hpp"

#if defined(DEBUGGER)
  #include "debugger/debugger.moc.hpp"
  #include "debugger/disassembler.moc.hpp"
  #include "debugger/breakpoint.moc.hpp"
  #include "debugger/hexeditor.moc.hpp"
  #include "debugger/memory.moc.hpp"
  #include "debugger/vramviewer.moc.hpp"
  #include "debugger/tracer.moc.hpp"
#endif

#include "input/input.hpp"

#include "link/filter.hpp"
#include "link/reader.hpp"

#include "movie/movie.hpp"

#include "settings/settings.moc.hpp"
#include "settings/pixelshader.moc.hpp"
#include "settings/video.moc.hpp"
#include "settings/audio.moc.hpp"
#include "settings/input.moc.hpp"
#include "settings/paths.moc.hpp"
#include "settings/advanced.moc.hpp"

#include "state/state.hpp"

#include "tools/tools.moc.hpp"
#include "tools/cheateditor.moc.hpp"
#include "tools/cheatfinder.moc.hpp"
#include "tools/statemanager.moc.hpp"
#include "tools/layertoggle.moc.hpp"

#include "utility/utility.hpp"

struct Style {
  static const char Monospace[64];

  enum {
    WindowMargin     = 5,
    WidgetSpacing    = 5,
    SeparatorSpacing = 5,
  };
};

#if !defined(PLATFORM_WIN)
  #define mkdir(path) (mkdir)(path, 0755)
#endif
