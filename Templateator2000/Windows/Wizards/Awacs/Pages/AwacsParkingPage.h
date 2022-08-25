#pragma once

#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>

class AwacsParkingPage final : public QWizardPage
{
	Q_OBJECT
	friend class AwacsWizard;

public:
	explicit AwacsParkingPage(QWidget* parent = nullptr);
	[[nodiscard]] virtual int nextId() const override;

private:
	QLabel* m_parkings_label;
	QComboBox* m_parkings_list;
	QVBoxLayout* m_layout;
};
