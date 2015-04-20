#include "office_view.h"
#include <QPainter>
#include <QDir>
#include <QFileInfo>
#include <QClipboard>
#include <QGuiApplication>
#include <QStaticText>
#include <windows.h>
#include "office.h"
#include "data/image_attr.h"


OfficeView::OfficeView(Office *office)
	: office{office},
	  mouseSelect{this, office},
	  word{nullptr},
	  excel{nullptr},
	  powerPoint{nullptr},
	  wordInitSucceed{false},
	  excelInitSucceed{false},
	  powerPointInitSucceed{false} {
	setObjectName("OfficeView");
	// setWindowFlags(Qt::ToolTip); //for video overlay
	setMouseTracking(true);
	unsetCursor();    // use parent cursor
    office->owner->view()->addWidget(this);
    office->owner->view()->setCurrentWidget(this);
	// setAttribute(Qt::WA_OpaquePaintEvent, true);
	currentOfficeFile = "";
	currentGraphicsStorePath = "";
	currentGraphicsCount = 0;
	currentGraphicsIndex = 1;
}

OfficeView::~OfficeView() {
	if (word) {
		word->dynamicCall("Quit()");
		delete word;
		word = nullptr;
	}
	if (excel) {
		excel->dynamicCall("Quit()");
		delete excel;
		excel = nullptr;
	}
	if (powerPoint) {
		powerPoint->dynamicCall("Quit()");
		delete powerPoint;
		powerPoint = nullptr;
	}
}

void OfficeView::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
    if (ImageAttr::BG_COLOR_NONE != office->attr()->bgColor) {
        painter.fillRect(rect(), QColor::fromRgba(office->attr()->bgColor));
	}
	if (!office->bgCache.isNull()) {
		painter.drawPixmap(Slice::POS_TOPLEFT, office->bgCache);
	}
	if (!office->cache->isNull())
		painter.drawPixmap(Slice::POS_TOPLEFT, *office->cache);
}

void OfficeView::resizeEvent(QResizeEvent *event) {
	office->cacheView();
	QWidget::resizeEvent(event);
}

QPixmap *OfficeView::drawOffice() {
	if (!office) {
		return nullptr;
	}
    QSize size(this->office->owner->attr()->geo.width,
               this->office->owner->attr()->geo.height);
	QPixmap *officePix = new QPixmap(size);
	officePix->fill(QColor::fromRgba(ImageAttr::BG_COLOR_NONE));
	QRectF region(Slice::POS_TOPLEFT, QSizeF(size));
	QPainter painter(officePix);
	painter.setRenderHint(QPainter::Antialiasing);
	drawOffice(painter, region);
	return officePix;
}

void OfficeView::drawOffice(QPainter &painter, const QRectF &region) {
    QString officeFilePath = QString::fromStdWString(office->attr()->file);
	if (!officeFilePath.isEmpty()) {
		QFileInfo officeFileInfo(officeFilePath);
		if (officeFileInfo.exists()) {
			QString officeSuffix = officeFileInfo.suffix();
			if (officeSuffix == "doc" || officeSuffix == "docx") {
				drawWord(painter, region, officeFilePath);
			} else if (officeSuffix == "xls" || officeSuffix == "xlsx") {
				drawExcel(painter, region, officeFilePath);
			} else if (officeSuffix == "ppt" || officeSuffix == "pptx") {
				drawPowerPoint(painter, region, officeFilePath);
			} else {
				QString officeTip = officeFilePath
									+ tr("is not a Microsoft Office file!");
				drawTipText(painter, region, officeTip);
			}
		} else {
			QString officeTip = officeFilePath + tr(" doesn't exist!");
			drawTipText(painter, region, officeTip);
		}
	} else {
		drawTipText(
			painter, region, tr("Please open a Microsoft Office file!"));
	}
}

void OfficeView::drawWord(QPainter &painter,
						  const QRectF &region,
						  const QString &file) {
	if (!word) {
		word = new QAxObject();
		wordInitSucceed = word->setControl("Word.Application");
	}
	if (wordInitSucceed) {
		painter.save();
		wordToGraphics(file, region);
		if (currentGraphicsIndex > currentGraphicsCount) {
			currentGraphicsIndex = 1;
		}
		QString framGraphicsNamePath;
		framGraphicsNamePath.setNum(currentGraphicsIndex);
		framGraphicsNamePath = getGraphicsStoreDir() + "/"
							   + office->wordPixPrefix + framGraphicsNamePath
							   + ".bmp";
		QFile framGraphicsFile(framGraphicsNamePath);
		if (framGraphicsFile.exists()) {
			QPixmap framGraphicsPix;
			framGraphicsPix.load(framGraphicsNamePath);
			framGraphicsPix = ImageTool::scaled(
                framGraphicsPix, office->attr()->scale, frameSize());
			painter.drawPixmap(Slice::POS_TOPLEFT, framGraphicsPix);
		} else {
			QString officeTip = framGraphicsNamePath + tr(" doesn't exist!");
			drawTipText(painter, region, officeTip);
		}
		++currentGraphicsIndex;
		painter.restore();
	} else {
		drawTipText(
			painter, region, tr("Please ensure  Microsoft Office installed!"));
	}
}

void OfficeView::drawExcel(QPainter &painter,
						   const QRectF &region,
						   const QString &file) {
	if (!excel) {
		excel = new QAxObject();
		excelInitSucceed = excel->setControl("Excel.Application");
	}
	if (excel && excelInitSucceed) {
		painter.save();
		excelToGraphics(file, region);
		if (currentGraphicsIndex > currentGraphicsCount) {
			currentGraphicsIndex = 1;
		}
		QString framGraphicsNamePath;
		framGraphicsNamePath.setNum(currentGraphicsIndex);
		framGraphicsNamePath = getGraphicsStoreDir() + "/"
							   + office->excelPixPrefix + framGraphicsNamePath
							   + ".bmp";
		QFile framGraphicsFile(framGraphicsNamePath);
		if (framGraphicsFile.exists()) {
			QPixmap framGraphicsPix;
			framGraphicsPix.load(framGraphicsNamePath);
			framGraphicsPix = ImageTool::scaled(
                framGraphicsPix, office->attr()->scale, frameSize());
			painter.drawPixmap(Slice::POS_TOPLEFT, framGraphicsPix);
		} else {
			QString officeTip = framGraphicsNamePath + tr(" doesn't exist!");
			drawTipText(painter, region, officeTip);
		}
		++currentGraphicsIndex;
		painter.restore();
	} else {
		drawTipText(
			painter, region, tr("Please ensure  Microsoft Office installed!"));
	}
}

void OfficeView::drawPowerPoint(QPainter &painter,
								const QRectF &region,
								const QString &file) {
	if (!powerPoint) {
		powerPoint = new QAxObject();
		powerPointInitSucceed
			= powerPoint->setControl("PowerPoint.Application");
	}
	if (powerPoint && powerPointInitSucceed) {
		painter.save();
		powerPointToGraphics(file, region);
		if (currentGraphicsIndex > currentGraphicsCount) {
			currentGraphicsIndex = 1;
		}
		QString framGraphicsNamePath;
		framGraphicsNamePath.setNum(currentGraphicsIndex);
		framGraphicsNamePath = getGraphicsStoreDir() + "/"
							   + office->powerPointPixPrefix
							   + framGraphicsNamePath + ".bmp";
		QFile framGraphicsFile(framGraphicsNamePath);
		if (framGraphicsFile.exists()) {
			QPixmap framGraphicsPix;
			framGraphicsPix.load(framGraphicsNamePath);
			framGraphicsPix = ImageTool::scaled(
                framGraphicsPix, office->attr()->scale, frameSize());
			painter.drawPixmap(Slice::POS_TOPLEFT, framGraphicsPix);
		} else {
			QString officeTip = framGraphicsNamePath + tr(" doesn't exist!");
			drawTipText(painter, region, officeTip);
		}
		++currentGraphicsIndex;
		painter.restore();
	} else {
		QString officeTip = tr("Please ensure  Microsoft Office installed!");
		drawTipText(painter, region, officeTip);
	}
}

void OfficeView::wordToGraphics(const QString &file, const QRectF &region) {
	if (currentOfficeFile != file) {
		currentGraphicsStorePath = getGraphicsStoreDir();
		if (!currentGraphicsStorePath.isEmpty()) {
			currentOfficeFile = file;
			QDir storeDir(currentGraphicsStorePath);
			if (storeDir.exists()) {
				storeDir.rmpath(currentGraphicsStorePath);
				storeDir.mkpath(currentGraphicsStorePath);
			} else {
				storeDir.mkpath(currentGraphicsStorePath);
			}
			currentGraphicsCount = 0;
			currentGraphicsIndex = 1;
			word->setProperty("Visible", false);
			QAxObject *documents = word->querySubObject("Documents");
			if (documents) {
				documents->dynamicCall("Open(QString)", file);
				QAxObject *selection = word->querySubObject("Selection");
				if (selection) {
					selection->dynamicCall("WholeStory()");
					int totleStart = selection->property("Start").toInt();
					int totleEnd = selection->property("End").toInt();
					int documentCount = documents->property("Count").toInt();
					if (documentCount > 0) {
						QAxObject *activeDocument
							= word->querySubObject("ActiveDocument");
						if (activeDocument) {
							int pageCount
								= selection->dynamicCall("Information(4)")
									  .toInt();
							currentGraphicsCount = pageCount;
							for (int index = 1; index <= pageCount; ++index) {
								int pageStart = 0;
								int pageEnd = 0;
								if (1 == pageCount) {
									pageStart = totleStart;
									pageEnd = totleEnd;
								} else {
									QList<QVariant> paraGoto;
									paraGoto << 1 << 1 << index;
									QAxObject *rangePre
										= selection->querySubObject(
											"GoTo(QVariant,QVariant,QVariant)",
											paraGoto);
									QAxObject *rangeNext
										= selection->querySubObject(
											"GoToNext(1)");
									if (rangePre && rangeNext) {
										pageStart = rangePre->property("Start")
														.toInt();
										if (pageCount == index) {
											pageEnd = totleEnd;
										} else {
											pageEnd
												= rangeNext->property("Start")
													  .toInt();
										}
									}
								}
								QList<QVariant> paraRange;
								paraRange << pageStart << pageEnd;
								QAxObject *pageRange
									= activeDocument->querySubObject(
										"Range(QVariant,QVariant)", paraRange);
								if (pageRange) {
									pageRange->dynamicCall("CopyAsPicture()");
									BYTE *clipImageBuff = nullptr;
									QImage clipImage = getClipboardData(
										reinterpret_cast<void **>(
											&clipImageBuff));
									if (!clipImage.isNull()) {
										QString pageImageName;
										pageImageName.setNum(index);
										pageImageName = office->wordPixPrefix
														+ pageImageName
														+ ".bmp";
										QString exportFilePath
											= currentGraphicsStorePath + "/"
											  + pageImageName;
										clipImage.save(exportFilePath);
										if (clipImageBuff) {
											delete clipImageBuff;
											clipImageBuff = nullptr;
										}    // clipImageBuff
									}        // clipboardImage isn't null
								}            // pageRange
							}                // for(;;)
						}                    // activeDocument
						QList<QVariant> paraClose;
						paraClose << 0 << 1 << false;
						activeDocument->dynamicCall(
							"Close(QVariant,QVariant,QVariant)", paraClose);
					}    // documentCount > 0
				}        // selection
			}            // documents
		}                // currentGraphicsStorePath isn't null
	}                    // currentOfficeFile != file
}

void OfficeView::excelToGraphics(const QString &file, const QRectF &region) {
	if (currentOfficeFile != file) {
		currentGraphicsStorePath = getGraphicsStoreDir();
		if (!currentGraphicsStorePath.isEmpty()) {
			currentOfficeFile = file;
			QDir storeDir(currentGraphicsStorePath);
			if (storeDir.exists()) {
				storeDir.rmpath(currentGraphicsStorePath);
				storeDir.mkpath(currentGraphicsStorePath);
			} else {
				storeDir.mkpath(currentGraphicsStorePath);
			}
			currentGraphicsCount = 0;
			currentGraphicsIndex = 1;
			excel->setProperty("Visible", false);
			QAxObject *workbooks = excel->querySubObject("workbooks");
			if (workbooks) {
				workbooks->dynamicCall("Open(QString)", file);
				QAxObject *workbook = workbooks->querySubObject("Item(int)", 1);
				if (workbook) {
					QAxObject *workSheets = workbook->querySubObject("Sheets");
					if (workSheets) {
						int sheetCount = workSheets->property("Count").toInt();
						currentGraphicsCount = sheetCount;
						if (sheetCount > 0) {
							for (int index = 1; index <= sheetCount; ++index) {
								QAxObject *workSheet
									= workSheets->querySubObject("Item(int)",
																 index);
								if (workSheet) {
									QAxObject *usedRange
										= workSheet->querySubObject(
											"UsedRange");
									if (usedRange) {
										QList<QVariant> paraCopyPicture;
										paraCopyPicture << 2 << 2;
										usedRange->dynamicCall("CopyPicture()");
										BYTE *clipImageBuff = nullptr;
										QImage clipImage = getClipboardData(
											reinterpret_cast<void **>(
												&clipImageBuff));
										if (!clipImage.isNull()) {
											QString sheetImageName;
											sheetImageName.setNum(index);
											sheetImageName
												= office->excelPixPrefix
												  + sheetImageName + ".bmp";
											QString exportFilePath
												= currentGraphicsStorePath + "/"
												  + sheetImageName;
											clipImage.save(exportFilePath);
											if (clipImageBuff) {
												delete clipImageBuff;
												clipImageBuff = nullptr;
											}    // clipImageBuff
										}        // clipboardImage isn't null
									}            // usedRange isn't null
								}                // workSheet isn't null
							}                    // for(;;)
						}                        // sheetCount > 0
					}                            // workSheets isn't null
				}                                // workbook isn't null
				workbooks->dynamicCall("Close()");
			}    // workbooks isn't null
		}        // currentGraphicsStorePath isn't null
	}            // currentOfficeFile
}

void OfficeView::powerPointToGraphics(const QString &file,
									  const QRectF &region) {
	if (currentOfficeFile != file) {
		currentGraphicsStorePath = getGraphicsStoreDir();
		if (!currentGraphicsStorePath.isEmpty()) {
			currentOfficeFile = file;
			QDir storeDir(currentGraphicsStorePath);
			if (storeDir.exists()) {
				storeDir.rmpath(currentGraphicsStorePath);
				storeDir.mkpath(currentGraphicsStorePath);
			} else {
				storeDir.mkpath(currentGraphicsStorePath);
			}
			currentGraphicsCount = 0;
			currentGraphicsIndex = 1;
			powerPoint->setProperty("Visible", false);
			QAxObject *presentations
				= powerPoint->querySubObject("Presentations");
			if (presentations) {
				QList<QVariant> paraOpen;
				paraOpen << currentOfficeFile << true << true << false;
				presentations->dynamicCall("Open(QString,bool,bool,bool)",
										   paraOpen);
				int presentationCount
					= presentations->property("Count").toInt();
				if (presentationCount > 0) {
					QAxObject *activePresention
						= presentations->querySubObject("Item(int)", 1);
					if (activePresention) {
						QAxObject *slides
							= activePresention->querySubObject("Slides");
						if (slides) {
							int slidesCount = slides->property("Count").toInt();
							currentGraphicsCount = slidesCount;
							int regionWidth = static_cast<int>(region.width());
							int regionHeight
								= static_cast<int>(region.height());
							for (int index = 1; index <= slidesCount; ++index) {
								QAxObject *slide = slides->querySubObject(
									"Item(int)", index);
								if (slide) {
									QString slideImageName;
									slideImageName.setNum(index);
									slideImageName = office->powerPointPixPrefix
													 + slideImageName + ".bmp";
									QList<QVariant> paraExport;
									QString exportFilePath
										= currentGraphicsStorePath + "/"
										  + slideImageName;
									paraExport << exportFilePath << "bmp"
											   << regionWidth << regionHeight;
									slide->dynamicCall(
										"Export(QString,QString,int,int)",
										paraExport);
								}    // slide
							}        // for(;;)
						}            // slides
						activePresention->dynamicCall("Close()");
					}    // activePresention
				}        // presentationCount > 0
			}            // presentations
		}                // currentGraphicsStorePath isn't empty
	}                    // currentOfficeFile
}

QImage OfficeView::getClipboardData(void **buff) {
	QImage clipImage;
	bool openClipboard = OpenClipboard(nullptr);
	if (openClipboard) {
		if (IsClipboardFormatAvailable(CF_ENHMETAFILE)) {
			HANDLE hClipData = GetClipboardData(CF_ENHMETAFILE);
			HENHMETAFILE hEnh = CopyEnhMetaFile((HENHMETAFILE)hClipData, NULL);
			HBITMAP hBmp = nullptr;
			HDC memDC = nullptr;
			ENHMETAHEADER emhMetaHeader;
			HDC dc = GetDC(NULL);
			ZeroMemory(&emhMetaHeader, sizeof(ENHMETAHEADER));
			bool ret = GetEnhMetaFileHeader(
				hEnh, sizeof(ENHMETAHEADER), &emhMetaHeader);
			if (ret != 0) {
				RECT rect;
				float pixX, pixY, mmX, mmY;
				long width, height;
				pixX = (float)GetDeviceCaps(dc, HORZRES);
				pixY = (float)GetDeviceCaps(dc, VERTRES);
				mmX = (float)GetDeviceCaps(dc, HORZSIZE);
				mmY = (float)GetDeviceCaps(dc, VERTSIZE);
				rect.top = (int)((float)emhMetaHeader.rclFrame.top * pixY
								 / (mmY * 100.0f));
				rect.left = (int)((float)emhMetaHeader.rclFrame.left * pixX
								  / (mmX * 100.0f));
				rect.right = (int)((float)emhMetaHeader.rclFrame.right * pixX
								   / (mmX * 100.0f));
				rect.bottom = (int)((float)emhMetaHeader.rclFrame.bottom * pixY
									/ (mmY * 100.0f));
				width = rect.right - rect.left;
				height = rect.bottom - rect.top;
				rect.top = 0;
				rect.left = 0;
				rect.right = width;
				rect.bottom = height;
				memDC = CreateCompatibleDC(dc);
				hBmp = CreateCompatibleBitmap(dc, width, height);
				SelectObject(memDC, hBmp);
				HBRUSH backBrush = (HBRUSH)(RGB(255, 255, 255));
				HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, backBrush);
				RECT clipRect;
				GetClipBox(memDC, &clipRect);
				PatBlt(memDC,
					   clipRect.left,
					   clipRect.top,
					   abs(clipRect.left - clipRect.right),
					   abs(clipRect.top - clipRect.bottom),
					   PATCOPY);
				SelectObject(memDC, oldBrush);
				bool pemf = PlayEnhMetaFile(memDC, hEnh, &rect);
				if (pemf) {
					BITMAP bmp;
					memset(&bmp, 0, sizeof(BITMAP));
					GetObject(hBmp, sizeof(BITMAP), &bmp);
					LONG bmpSize = bmp.bmHeight * bmp.bmWidthBytes;
					*buff = reinterpret_cast<void *>(new BYTE[bmpSize]);
					GetBitmapBits(hBmp, bmpSize, *buff);
					clipImage = QImage(reinterpret_cast<uchar *>(*buff),
									   bmp.bmWidth,
									   bmp.bmHeight,
									   bmp.bmWidthBytes,
									   QImage::Format_RGB32);
				}
			}
			DeleteEnhMetaFile(hEnh);
		}
		CloseClipboard();
	}
	return clipImage;
}

void OfficeView::drawTipText(QPainter &painter,
							 const QRectF &region,
							 const QString &tip) {
	painter.save();
	QFont font;
	font.setPixelSize(office->tipFontSize);
	QFontMetricsF tipMetrics(font);
	QRectF tipRect = tipMetrics.boundingRect(tip);
	qreal tipWidth = tipRect.width();
	qreal tipHeight = tipRect.height();
	QPointF paintPoint((region.width() - tipWidth) / 2,
					   (region.height() - tipHeight) / 2);
	painter.setPen(QColor::fromRgba(ImageAttr::OPAQUE_RED));
	painter.setFont(font);
	QStaticText staticText(tip);
	QTextOption textOption;
	textOption.setAlignment(Qt::AlignCenter);
	staticText.setTextOption(textOption);
	painter.drawStaticText(paintPoint, staticText);
	painter.restore();
}

QString OfficeView::getGraphicsStoreDir() {
	QString graphocsStoreDir;
	QString officeFilePath = gerOfficeFilePath();
	QString appPath = QCoreApplication::applicationDirPath();
	if (!officeFilePath.isEmpty()) {
		QFileInfo officeFileInfo(officeFilePath);
		if (officeFileInfo.exists()) {
			QString officeSuffix = officeFileInfo.suffix();
			if (officeSuffix == "doc" || officeSuffix == "docx") {
				graphocsStoreDir = appPath + "/tmp/office/word/"
								   + officeFileInfo.baseName();
			} else if (officeSuffix == "xls" || officeSuffix == "xlsx") {
				graphocsStoreDir = appPath + "/tmp/office/excel/"
								   + officeFileInfo.baseName();
			} else if (officeSuffix == "ppt" || officeSuffix == "pptx") {
				graphocsStoreDir = appPath + "/tmp/office/ppt/"
								   + officeFileInfo.baseName();
			} else {
				graphocsStoreDir = "";
			}
		} else {
			graphocsStoreDir = "";
		}
	} else {
		graphocsStoreDir = "";
	}
	return graphocsStoreDir;
}

QString OfficeView::gerOfficeFilePath() {
    return QString::fromStdWString(office->attr()->file);
}
