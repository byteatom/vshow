#-------------------------------------------------
#
# Project created by QtCreator 2014-08-05T17:43:00
#
#-------------------------------------------------

QT += core gui network axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vshow
TEMPLATE = app
VERSION = 1.0.0.0

CONFIG -= debug_and_release
CONFIG += c++11

CONFIG(debug, debug|release) {
	BUILD_CFG=debug
	BUILD_CFG_SUFFIX=D
} else {
	BUILD_CFG=release
	BUILD_CFG_SUFFIX=
}

BUILD_NAME = $${TARGET}-$${QMAKE_TARGET.arch}-$${BUILD_CFG}-qt$${QT_MAJOR_VERSION}.$${QT_MINOR_VERSION}
DESTDIR = $$_PRO_FILE_PWD_/../run/$${BUILD_NAME}
TMPDIR = $$_PRO_FILE_PWD_/../tmp/$${BUILD_NAME}
OBJECTS_DIR = $$TMPDIR/obj
MOC_DIR = $$TMPDIR/moc
RCC_DIR = $$TMPDIR/qrc
UI_DIR = $$TMPDIR/ui

win32 {
	DEFINES += _WINSOCK_DEPRECATED_NO_WARNINGS \
				_WIN32_WINNT=0x0501 \
				_CRT_SECURE_NO_WARNINGS
	LIBS += -lWinmm -lIphlpapi -lversion
	QMAKE_LFLAGS_WINDOWS += /SUBSYSTEM:WINDOWS\",5.01\" /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
	QMAKE_CXXFLAGS_WARN_ON += /wd4100 /wd4200
}


INCLUDEPATH +=	$$_PRO_FILE_PWD_/

INCLUDEPATH +=	$$_PRO_FILE_PWD_/../lib/boost_1_57_0
LIBS += -L$$_PRO_FILE_PWD_/../lib/boost_1_57_0/stage/$$QMAKE_TARGET.arch


INCLUDEPATH +=	$$_PRO_FILE_PWD_/../lib/vlc-qt-0.10.0/include
LIBS += -L$$_PRO_FILE_PWD_/../lib/vlc-qt-0.10.0/lib-$$QMAKE_TARGET.arch \
		-lvlc-qt \
		-lvlc-qt-widgets

INCLUDEPATH +=	$$_PRO_FILE_PWD_/../lib/xerces-3.1.1/include
LIBS += -L$$_PRO_FILE_PWD_/../lib/xerces-3.1.1/$$QMAKE_TARGET.arch \
		-lxerces-c_3$$BUILD_CFG_SUFFIX

INCLUDEPATH +=	$$_PRO_FILE_PWD_/../lib/winpcap-4.1.3/include
LIBS += -L$$_PRO_FILE_PWD_/../lib/winpcap-4.1.3/$$QMAKE_TARGET.arch \
		-lwpcap \
		-lPacket

message($$INCLUDEPATH)
message($$LIBS)


SOURCES += \
	data/data.cpp \
	data/app_attr.cpp \
	ui/text.cpp \
	data/program_attr.cpp \
	data/region_attr.cpp \
	data/screen_attr.cpp \
	data/segment_attr.cpp \
	data/text_attr.cpp \
	ui/program.cpp \
	ui/region.cpp \
	ui/screen.cpp \
	ui/segment.cpp \
	data/slice_attr.cpp \
	data/project_attr.cpp \
	ui/project.cpp \
	ui/slice.cpp \
	ui/mouse_select.cpp \
	data/picture_attr.cpp \
	ui/picture.cpp \
	ui/program_view.cpp \
	data/animate_attr.cpp \
	data/video_attr.cpp \
	ui/video.cpp \
	ui/video_view_vlc.cpp \
	ui/animate_cfger.cpp \
	ui/card_ui.cpp \
	ui/attr_cfger.cpp \
	ui/filesrc_cfger.cpp \
	ui/main_ui.cpp \
	ui/picture_view.cpp \
	ui/program_cfger.cpp \
	ui/region_view.cpp \
	ui/screen_cfger.cpp \
	ui/screen_view.cpp \
	ui/segment_cfger.cpp \
	ui/slice_cfger.cpp \
	ui/text_cfger.cpp \
	ui/text_view.cpp \
	ui/video_cfger.cpp \
	ui/play_ctrl.cpp \
	ui/program_list.cpp \
	qlog/qlog.cpp \
	qlog/sender.cpp \
	vshow/vshow.cpp \
	ui/text_editor.cpp \
	ui/image_cfger.cpp \
	data/image_attr.cpp \
	io/asio_prot.cpp \
	io/serial_prot.cpp \
	io/udp_prot.cpp \
	ui/animator.cpp \
	ui/geo_cfger.cpp \
	data/geo_attr.cpp \
	io/protocol.cpp \
	io/card_msg.cpp \
	io/eth_msg.cpp \
	io/msg_buf.cpp \
	io/card_prot.cpp \
	io/eth_prot.cpp \
	io/io.cpp \
	util/endian.cpp \
	util/image_tool.cpp \
	util/media_player_vlc.cpp \
	util/mouse_geo.cpp \
	util/xml_dom.cpp \
	util/task_runner.cpp \
	util/task_syncer.cpp \
	util/enumser.cpp \
	util/hi_timer.cpp \
	ui/scan_ui.cpp \
	data/clock_attr.cpp \
	data/office_attr.cpp \
	data/timer_attr.cpp \
	data/watch_attr.cpp \
	ui/clock.cpp \
	ui/clock_cfger.cpp \
	ui/clock_view.cpp \
	ui/office.cpp \
	ui/office_view.cpp \
	ui/timer.cpp \
	ui/timer_cfger.cpp \
	ui/timer_view.cpp \
	ui/watch.cpp \
	ui/watch_cfger.cpp \
	ui/watch_view.cpp \
	data/card.cpp \
	data/sender_card.cpp \
	util/tree_node.cpp \
	ui/link_ui.cpp \
	ui/link_item.cpp \
	ui/leak_spinbox.cpp \
	data/recver_card.cpp \
	util/registry.cpp \
	util/mem_util.cpp \
	util/qt_util.cpp \
	util/win_util.cpp \
	io/eth_caster.cpp \
	data/card_manager.cpp \
	ui/about_dialog.cpp

HEADERS +=\
	data/data.h \
	io/protocol.h \
	ui/program.h \
	ui/region.h \
	ui/screen.h \
	ui/segment.h \
	ui/text.h \
	ui/project.h \
	ui/slice.h \
	ui/mouse_select.h \
	ui/picture.h \
	ui/program_view.h \
	ui/video.h \
	ui/video_view_vlc.h \
	data/animate_attr.h \
	data/app_attr.h \
	data/image_attr.h \
	data/picture_attr.h \
	data/program_attr.h \
	data/project_attr.h \
	data/region_attr.h \
	data/screen_attr.h \
	data/segment_attr.h \
	data/slice_attr.h \
	data/text_attr.h \
	data/video_attr.h \
	ui/animate_cfger.h \
	ui/card_ui.h \
	ui/attr_cfger.h \
	ui/filesrc_cfger.h \
	ui/main_ui.h \
	ui/picture_view.h \
	ui/program_cfger.h \
	ui/region_view.h \
	ui/screen_cfger.h \
	ui/screen_view.h \
	ui/segment_cfger.h \
	ui/slice_cfger.h \
	ui/text_cfger.h \
	ui/text_view.h \
	ui/video_cfger.h \
	ui/play_ctrl.h \
	ui/program_list.h \
	ui/media_player.h \
	ui/video_view.h \
	util/fsm.h \
	qlog/qlog.h \
	qlog/sender.h \
	vshow/vshow.h \
	ui/text_editor.h \
	data/attr.h \
	ui/image_cfger.h \
	io/asio_prot.h \
	io/serial_prot.h \
	io/udp_prot.h \
	ui/animator.h \
	ui/geo_cfger.h \
	data/geo_attr.h \
	io/card_msg.h \
	io/eth_msg.h \
	io/msg_buf.h \
	io/card_prot.h \
	io/eth_prot.h \
	io/io.h \
	util/endian.h \
	util/image_tool.h \
	util/media_player_vlc.h \
	util/mouse_geo.h \
	util/xml_dom.h \
	util/task_runner.h \
	util/task_syncer.h \
	util/enumser.h \
	util/hi_timer.h \
	ui/scan_ui.h \
	data/clock_attr.h \
	data/office_attr.h \
	data/timer_attr.h \
	data/watch_attr.h \
	ui/clock.h \
	ui/clock_cfger.h \
	ui/clock_view.h \
	ui/office.h \
	ui/office_view.h \
	ui/timer.h \
	ui/timer_cfger.h \
	ui/timer_view.h \
	ui/watch.h \
	ui/watch_cfger.h \
	ui/watch_view.h \
	data/card.h \
	data/sender_card.h \
	util/tree_node.h \
	ui/link_ui.h \
	ui/link_item.h \
	ui/leak_spinbox.h \
	data/recver_card.h \
	util/registry.h \
	util/qt_util.h \
	util/mem_util.h \
	util/win_util.h \
	util/task.h \
	io/eth_caster.h \
	util/tlv.h \
	data/card_manager.h \
	ui/about_dialog.h

FORMS += \
	ui/program_view.ui \
	ui/main_ui.ui \
	ui/card_ui.ui \
	ui/animate_cfger.ui \
	ui/filesrc_cfger.ui \
	ui/program_cfger.ui \
	ui/region_view.ui \
	ui/screen_cfger.ui \
	ui/screen_view.ui \
	ui/segment_cfger.ui \
	ui/slice_cfger.ui \
	ui/text_cfger.ui \
	ui/video_cfger.ui \
	ui/play_ctrl.ui \
	ui/program_list.ui \
	ui/image_cfger.ui \
	ui/geo_cfger.ui \
	ui/scan_ui.ui \
	ui/clock_cfger.ui \
	ui/timer_cfger.ui \
	ui/watch_cfger.ui \
	ui/link_ui.ui \
	ui/link_item.ui \
	ui/about_dialog.ui

RESOURCES += \
	res/vshow.qrc

TRANSLATIONS += vshow/vshow_zh_CN.ts
