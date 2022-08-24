#pragma once

#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QWizardPage>

class AtisRadioPage final : public QWizardPage
{
	Q_OBJECT
	friend class AtisWizard;

public:
	explicit AtisRadioPage(QWidget* parent = nullptr);

	virtual int nextId() const override;

	[[nodiscard]] std::vector<std::string> Frequencies() const;

private slots:
	void addRadioFrequency();
	void rmRadioFrequency() const;

private:
	static QRegularExpressionValidator* m_frequency_validator;

	QLabel* m_label;
	QListWidget* m_frequencies;
	QPushButton* m_add_fq_btn;
	QPushButton* m_rm_fq_btn;
	QGridLayout* m_layout;
};
