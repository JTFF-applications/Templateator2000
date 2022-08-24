#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWizardPage>

class AtisSrsPage final : public QWizardPage
{
	Q_OBJECT
	friend class AtisWizard;

public:
	explicit AtisSrsPage(QWidget* parent = nullptr);

	virtual bool validatePage() override;

signals:
	// ReSharper disable once CppInconsistentNaming
	void finished();

private:
	QLabel* m_label;
	QLineEdit* m_srs_path;
	QVBoxLayout* m_layout;
};
