#pragma once

#include <QValidator>

class QRealIntValidator final : public QValidator
{
public:
	explicit QRealIntValidator(const int& min = INT_MIN, const int& max = INT_MAX, QObject* parent = nullptr);
	virtual ~QRealIntValidator() override = default;

	QRealIntValidator(const QRealIntValidator&) = delete;
	QRealIntValidator(const QRealIntValidator&&) = delete;
	QRealIntValidator& operator=(const QRealIntValidator& other) = delete;
	QRealIntValidator& operator=(const QRealIntValidator&& other) = delete;

	virtual State validate(QString& input, int& pos) const override;

private:
	const int m_min, m_max;
};
