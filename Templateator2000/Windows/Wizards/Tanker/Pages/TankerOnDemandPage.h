#pragma once

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWizardPage>

class TankerOnDemandPage final : public QWizardPage
{
	Q_OBJECT
	friend class TankerWizard;

public:
	explicit TankerOnDemandPage(QWidget* parent = nullptr);

	virtual void initializePage() override;
	virtual bool validatePage() override;
	[[nodiscard]] virtual int nextId() const override;

private:
	QLabel* m_degrees_label;
	QLabel* m_nm_label;
	QLabel* m_orbit_heading_label;
	QLineEdit* m_orbit_heading;
	QLabel* m_orbit_heading_error;
	QLabel* m_orbit_length_label;
	QLineEdit* m_orbit_length;
	QLabel* m_orbit_length_error;
	QGridLayout* m_layout;
};
