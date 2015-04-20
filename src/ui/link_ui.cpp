#include "link_ui.h"
#include "ui_link_ui.h"

const LinkUi::StyleOps LinkUi::styleOps[] =
{
	{&LinkUi::style0Init, &LinkUi::style0Step},
	{&LinkUi::style1Init, &LinkUi::style1Step},
	{&LinkUi::style2Init, &LinkUi::style2Step},
	{&LinkUi::style3Init, &LinkUi::style3Step},
	{&LinkUi::style4Init, &LinkUi::style4Step},
	{&LinkUi::style5Init, &LinkUi::style5Step},
	{&LinkUi::style6Init, &LinkUi::style6Step},
	{&LinkUi::style7Init, &LinkUi::style7Step},
	{&LinkUi::style8Init, &LinkUi::style8Step}
};

LinkUi::LinkUi(QWidget *parent, RecverCard::Proxy &proxy) :
	QDialog(parent),
	ui(new Ui::LinkUi),
	proxy{proxy}
{
	ui->setupUi(this);

	sink = std::bind(&LinkUi::itemClicked, this,
					 std::placeholders::_1,
					 std::placeholders::_2,
					 std::placeholders::_3);

	assert(proxy.rows * proxy.cols <= RecverCard::RCVER_CARDS_MAX);

	ui->rows->setValue(proxy.rows);
	ui->rows->setMaximum(RecverCard::RCVER_CARDS_MAX);
	connect(ui->rows, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &LinkUi::dimChanged);

	ui->cols->setValue(proxy.cols);
	ui->cols->setMaximum(RecverCard::RCVER_CARDS_MAX);
	connect(ui->cols, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &LinkUi::dimChanged);

	styleBtns.addButton(ui->style0, RecverCard::LINK_0);
	styleBtns.addButton(ui->style1, RecverCard::LINK_1);
	styleBtns.addButton(ui->style2, RecverCard::LINK_2);
	styleBtns.addButton(ui->style3, RecverCard::LINK_3);
	styleBtns.addButton(ui->style4, RecverCard::LINK_4);
	styleBtns.addButton(ui->style5, RecverCard::LINK_5);
	styleBtns.addButton(ui->style6, RecverCard::LINK_6);
	styleBtns.addButton(ui->style7, RecverCard::LINK_7);
	styleBtns.addButton(ui->style8, RecverCard::LINK_8);
	styleBtns.button(proxy.style)->setChecked(true);
	connect(&styleBtns, static_cast<void (QButtonGroup::*)(int id)>
			(&QButtonGroup::buttonClicked), this, &LinkUi::styleChanged);

	connect(ui->link, &QTableWidget::cellClicked,
			this, &LinkUi::cellClicked);

	connect(ui->ok, &QPushButton::clicked,
			this, &LinkUi::finish);

	connect(ui->cancel, &QPushButton::clicked,
			this, &LinkUi::cancel);

	load();
}

LinkUi::~LinkUi()
{
	delete ui;
}

void LinkUi::load()
{
	ui->link->setRowCount(proxy.rows);
	ui->link->setColumnCount(proxy.cols);

	LinkItem *item;
	for (size_t i = 0; i < proxy.rows * proxy.cols; ++i) {
		item = new LinkItem(proxy.dims[i].row, proxy.dims[i].col, this);
		item->index->setMaximum(RecverCard::RCVER_CARDS_MAX);
		item->index->setValue(i + 1);
		item->width->setMaximum(RecverCard::WIDTH_MAX);
		item->width->setValue(proxy.geos[i].width);
		item->height->setMaximum(RecverCard::HEIGHT_MAX);
		item->height->setValue(proxy.geos[i].height);

		ui->link->setCellWidget(proxy.dims[i].row, proxy.dims[i].col, item);
	}
}

void LinkUi::zoom()
{
	int rows = ui->rows->value();
	int cols = ui->cols->value();

	ui->link->setRowCount(rows);
	ui->link->setColumnCount(cols);

	LinkItem *item;
	LinkItem *itemPre;
	for (int col = 0; col < cols; ++col) {
		for (int row = 0; row < rows; ++row) {
			item = static_cast<LinkItem *>(ui->link->cellWidget(row, col));
			if (!item) {
				item = new LinkItem(row, col, this);
				item->index->setMaximum(RecverCard::RCVER_CARDS_MAX);

				item->width->setMaximum(RecverCard::WIDTH_MAX);
				itemPre = static_cast<LinkItem *>(ui->link->cellWidget(row - 1, col));
				item->width->setValue(itemPre
									  ? itemPre->width->value()
									  : RecverCard::WIDTH_DEFAULT);

				item->height->setMaximum(RecverCard::HEIGHT_MAX);
				itemPre = static_cast<LinkItem *>(ui->link->cellWidget(row, col - 1));
				item->height->setValue(itemPre
									   ? itemPre->height->value()
									   : RecverCard::HEIGHT_DEFAULT);

				ui->link->setCellWidget(row, col, item);
			}
		}
	}
	sort();
}

void LinkUi::sort()
{
	QPoint pos;
	styleInit(pos);
	LinkItem *item;
	for (int i = 0; i < ui->rows->value() * ui->cols->value(); ++i) {
		item = static_cast<LinkItem *>(ui->link->cellWidget(pos.y(), pos.x()));
		item->index->setValue(i + 1);
		styleStep(pos);
	}
}

void LinkUi::save()
{
	int rows = ui->rows->value();
	int cols = ui->cols->value();

	proxy.rows = rows;
	proxy.cols = cols;
	proxy.style = styleBtns.checkedId();

	LinkItem *item;
	LinkItem *itemPre;
	int index;
	int indexPre;
	for (int col = 0; col < cols; ++col) {
		for (int row = 0; row < rows; ++row) {
			item = static_cast<LinkItem *>(ui->link->cellWidget(row, col));
			index = item->index->value() - 1;
			proxy.dims[index].row = row;
			proxy.dims[index].col = col;
			proxy.geos[index].width = item->width->value();
			proxy.geos[index].height = item->height->value();
			if (0 == col)
				proxy.geos[index].x = 0;
			else {
				itemPre = static_cast<LinkItem *>(ui->link->cellWidget(row, col - 1));
				indexPre = itemPre->index->value() - 1;
				proxy.geos[index].x = proxy.geos[indexPre].x
						+ proxy.geos[indexPre].width;
			}
			if (0 == row)
				proxy.geos[index].y = 0;
			else {
				itemPre = static_cast<LinkItem *>(ui->link->cellWidget(row - 1, col));
				indexPre = itemPre->index->value() - 1;
				proxy.geos[index].y = proxy.geos[indexPre].y
						+ proxy.geos[indexPre].height;
			}
		}
	}
}

void LinkUi::widthChanged(LinkItem *item, int value)
{
	for (int row = 0; row < ui->rows->value(); ++row) {
		if (row == item->row) continue;
		LinkItem *cousin = static_cast<LinkItem *>(
					ui->link->cellWidget(row, item->col));
		cousin->width->blockSignals(true);
		cousin->width->setValue(value);
		cousin->width->blockSignals(false);
	}
}

void LinkUi::heightChanged(LinkItem *item, int value)
{
	for (int col = 0; col < ui->cols->value(); ++col) {
		if (col == item->col) continue;
		LinkItem *cousin = static_cast<LinkItem *>(
					ui->link->cellWidget(item->row, col));
		cousin->height->blockSignals(true);
		cousin->height->setValue(value);
		cousin->height->blockSignals(false);
	}
}

void LinkUi::dimChanged(int value)
{
	zoom();
}

void LinkUi::styleChanged(int id)
{
	sort();
}

void LinkUi::cellClicked(int row, int col)
{
	LinkItem *item = static_cast<LinkItem *>(ui->link->cellWidget(row, col));
	item->setStyleSheet("background-color:transparent;");
}


void LinkUi::itemClicked(bool in, int row, int col)
{
	if (!in) return;
}

void LinkUi::finish()
{
	save();
	accept();
}

void LinkUi::cancel()
{
	reject();
}

void LinkUi::styleInit(QPoint &pos)
{
	(this->*(styleOps[styleBtns.checkedId()].init))(pos);
}

void LinkUi::style0Init(QPoint &pos)
{
	pos = {0, 0};
}

void LinkUi::style1Init(QPoint &pos)
{
	pos = {0, 0};
}

void LinkUi::style2Init(QPoint &pos)
{
	style1Init(pos);
}

void LinkUi::style3Init(QPoint &pos)
{
	pos = {ui->cols->value() - 1, 0};
}

void LinkUi::style4Init(QPoint &pos)
{
	pos = {ui->cols->value() - 1, 0};
}

void LinkUi::style5Init(QPoint &pos)
{
	pos = {0, ui->rows->value() - 1};
}

void LinkUi::style6Init(QPoint &pos)
{
	pos = {0, ui->rows->value() - 1};
}

void LinkUi::style7Init(QPoint &pos)
{
	pos = {ui->cols->value() - 1,  ui->rows->value() - 1};
}

void LinkUi::style8Init(QPoint &pos)
{
	pos = {ui->cols->value() - 1,  ui->rows->value() - 1};
}

void LinkUi::styleStep(QPoint &pos)
{
	(this->*(styleOps[styleBtns.checkedId()].step))(pos);
}

void LinkUi::style0Step(QPoint &pos)
{
}

void LinkUi::style1Step(QPoint &pos)
{
	if (pos.x() % 2 == 0) {
		if (pos.y() < ui->rows->value() - 1)
			pos.setY(pos.y() + 1);
		else
			pos.setX(pos.x() + 1);
	} else {
		if (pos.y() > 0)
			pos.setY(pos.y() - 1);
		else
			pos.setX(pos.x() + 1);
	}
}

void LinkUi::style2Step(QPoint &pos)
{
	if (pos.y() % 2 == 0) {
		if (pos.x() < ui->cols->value() - 1)
			pos.setX(pos.x() + 1);
		else
			pos.setY(pos.y() + 1);
	} else {
		if (pos.x() > 0)
			pos.setX(pos.x() - 1);
		else
			pos.setY(pos.y() + 1);
	}
}

void LinkUi::style3Step(QPoint &pos)
{
	if (pos.x() % 2 == (ui->cols->value() - 1) % 2) {
		if (pos.y() < ui->rows->value() - 1)
			pos.setY(pos.y() + 1);
		else
			pos.setX(pos.x() - 1);
	} else {
		if (pos.y() > 0)
			pos.setY(pos.y() - 1);
		else
			pos.setX(pos.x() - 1);
	}
}

void LinkUi::style4Step(QPoint &pos)
{
	if (pos.y() % 2 == 0) {
		if (pos.x() > 0)
			pos.setX(pos.x() - 1);
		else
			pos.setY(pos.y() + 1);
	} else {
		if (pos.x() < ui->cols->value() - 1)
			pos.setX(pos.x() + 1);
		else
			pos.setY(pos.y() + 1);
	}
}

void LinkUi::style5Step(QPoint &pos)
{
	if (pos.x() % 2 == 0) {
		if (pos.y() > 0)
			pos.setY(pos.y() - 1);
		else
			pos.setX(pos.x() + 1);
	} else {
		if (pos.y() < ui->rows->value() - 1)
			pos.setY(pos.y() + 1);
		else
			pos.setX(pos.x() + 1);
	}
}

void LinkUi::style6Step(QPoint &pos)
{
	if (pos.y() % 2 == (ui->rows->value() - 1) % 2) {
		if (pos.x() < ui->cols->value() - 1)
			pos.setX(pos.x() + 1);
		else
			pos.setY(pos.y() - 1);
	} else {
		if (pos.x() > 0)
			pos.setX(pos.x() - 1);
		else
			pos.setY(pos.y() - 1);
	}
}

void LinkUi::style7Step(QPoint &pos)
{
	if (pos.x() % 2 == (ui->cols->value() - 1) % 2) {
		if (pos.y() > 0)
			pos.setY(pos.y() - 1);
		else
			pos.setX(pos.x() - 1);
	} else {
		if (pos.y() < ui->rows->value() - 1)
			pos.setY(pos.y() + 1);
		else
			pos.setX(pos.x() - 1);
	}
}

void LinkUi::style8Step(QPoint &pos)
{
	if (pos.y() % 2 == (ui->rows->value() - 1) % 2) {
		if (pos.x() > 0)
			pos.setX(pos.x() - 1);
		else
			pos.setY(pos.y() - 1);
	} else {
		if (pos.x() < ui->cols->value() - 1)
			pos.setX(pos.x() + 1);
		else
			pos.setY(pos.y() - 1);
	}
}
