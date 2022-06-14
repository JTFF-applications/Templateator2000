#pragma once

#include <QValidator>

class QStringListValidator : public QValidator
{
public:
	QStringListValidator(const QStringList& values, QObject* parent = nullptr);
	~QStringListValidator() = default;

	virtual State validate(QString& input, int& pos) const override;

private:
	const QStringList m_values;
};
