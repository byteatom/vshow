#ifndef SCAN_UI_H
#define SCAN_UI_H

#include <list>

#include <QWizard>
#include <QButtonGroup>

#include <data/recver_card.h>

namespace Ui {
class ScanUi;
}

class ScanUi : public QWizard
{
	Q_OBJECT

public:
	explicit ScanUi(QWidget *parent, RecverCard *card);
	~ScanUi();

	virtual void done(int r) override;

protected:
	void initializePage(int id) override;   //before show caused by next
	void cleanupPage(int id) override;      //before leave caused by back
	bool validateCurrentPage() override;    //when next or finish
	void pageEnter(int id);
	void pageLeave(int id);

private:
	typedef void (ScanUi::*PageAction)();
	static const PageAction enterActions[];

	void setTune();

	void enterModule();
	void baseColorChanged(int index);
	void widthChanged(int value);
	void heightChanged(int value);
	void hubChanged(bool checked);
	void chip138Changed(bool checked);

	void enterDataPolarity();
	void dataPolarityChanged(bool checked);
	void dataColorChanged(int id);

	void enterOePolarity();
	void oePolarityChanged(bool checked);
	void oeBrightChanged(int id);

	void enterRgbSel();
	void rgbSelTuneChanged(int id);
	void rgbSelChanged(int index);

	void enterIntfLines();
	void intfLinesChanged(int value);

	void enterWrapTimes();
	void wrapTimesChanged(int value);

	void enterDotmap();
	void setDot();
	void addDot(int row, int col);
	void clickDot(int row, int col);
	void undoDot(bool checked);
	void resetDotMap(bool checked);

	RecverCard *card;
	RecverCard::Scan &scan;
	RecverCard::Scan &tune;
	RecverCard::DotPos *dotsPos;
	int nextDot;

	QButtonGroup dataPolarityBtns;
	QButtonGroup oePolarityBtns;
	QButtonGroup colorBtns;
	Ui::ScanUi *ui;
};

#endif // SCAN_UI_H
