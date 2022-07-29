#pragma once

#include <QDialog>

#include "ui_AboutDialog.h"

class AboutDialog final : public QDialog
{
	Q_OBJECT

public:
	explicit AboutDialog(QWidget* parent = nullptr);

private:
	Ui::AboutDialogClass m_ui = {};
};
