#pragma once

#include <QValidator>

class QStringListValidator final : public QValidator
{
public:
	explicit QStringListValidator(const QStringList& values, QObject* parent = nullptr);
	virtual ~QStringListValidator() override = default;

	QStringListValidator(const QStringListValidator&) = delete;
	QStringListValidator(const QStringListValidator&&) = delete;
	QStringListValidator& operator=(const QStringListValidator& other) = delete;
	QStringListValidator& operator=(const QStringListValidator&& other) = delete;

	virtual State validate(QString& input, int& pos) const override;

private:
	const QStringList m_values;
};
