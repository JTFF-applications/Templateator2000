#include "Windows/AboutDialog.h"

AboutDialog::AboutDialog(QWidget* parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	connect(m_ui.close_btn, &QPushButton::clicked, this, &AboutDialog::close);
}
