#ifndef UNITSPINBOX_H
#define UNITSPINBOX_H

#include <QDoubleSpinBox>

class UnitConverter;

class UnitSpinBox : public QDoubleSpinBox
{
	Q_OBJECT
	public:
		explicit UnitSpinBox(QWidget *parent = nullptr);

	protected:
		QString textFromValue(double Value) const override;
		double valueFromText(const QString &text) const override;
		void stepBy(int steps) override;

	private slots:
		void onUnitSystemChanged();

	private:
		const UnitConverter *m_uc;
};

#endif // UNITSPINBOX_H
