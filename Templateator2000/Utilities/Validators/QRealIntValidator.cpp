#include "QRealIntValidator.h"

QRealIntValidator::QRealIntValidator(const int& min, const int& max, QObject* parent)
	: QValidator(parent), m_min(min), m_max(max)
{
}

QValidator::State QRealIntValidator::validate(QString& input, int& pos) const
{
	bool is_ok = true;
	const int nb = input.toInt(&is_ok);
	if (!is_ok)
		return Invalid;

	if (nb >= m_min && nb <= m_max)
		return Acceptable;
	return Invalid;
}
