#include "unitspinbox.h"
#include "../units.h"

UnitSpinBox::UnitSpinBox(QWidget *parent)
	: QDoubleSpinBox(parent)
	, m_uc(UnitConverter::instance())
{
	setDecimals(2);
	connect(m_uc, &UnitConverter::unitSystemChanged,
	        this, &UnitSpinBox::onUnitSystemChanged);
}

void UnitSpinBox::onUnitSystemChanged()
{
	setValue(value());
}

QString UnitSpinBox::textFromValue(double Value) const
{
	return QDoubleSpinBox::textFromValue(m_uc->toDisplay(Value));
}

double UnitSpinBox::valueFromText(const QString &text) const
{
	return m_uc->fromDisplay(QDoubleSpinBox::valueFromText(text));
}

void UnitSpinBox::stepBy(int steps)
{
	setValue(value() + steps * m_uc->fromDisplay(1.0));
}
