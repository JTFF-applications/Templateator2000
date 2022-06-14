#include "Utilities/Validators/QStringListValidator.h"

QStringListValidator::QStringListValidator(const QStringList& values, QObject* parent)
	: QValidator(parent), m_values(values)
{
}

QValidator::State QStringListValidator::validate(QString& input, int& pos) const
{
	if (input.isEmpty())
		return Acceptable;

	if (m_values.contains(input))
		return Acceptable;

	for (const auto& value : m_values)
		if (value.startsWith(input))
			return Acceptable;

	return Invalid;
}
