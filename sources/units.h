#ifndef UNITS_H
#define UNITS_H

#include <QtGlobal>
#include <QString>
#include <QObject>

namespace QET {
	enum UnitSystem {
		None       = 0,
		Inch       = 1,
		Foot       = 2,
		Mile       = 3,
		Millimeter = 4,
		Centimeter = 5,
		Meter      = 6,
		Kilometer  = 7,
		Microinch  = 8,
		Mil        = 9,
		Yard       = 10,
		Angstrom   = 11,
		Nanometer  = 12,
		Micron     = 13,
		Decimeter  = 14,
		Decameter  = 15,
		Hectometer = 16,
		Gigameter  = 17,
		Astro      = 18,
		Lightyear  = 19,
		Parsec     = 20,
		MaxUnit    = 21
	};
}

class UnitConverter : public QObject
{
	Q_OBJECT
public:
	static UnitConverter *instance();
	static void dropInstance();

	QET::UnitSystem system() const;
	void setSystem(QET::UnitSystem sys);

	/// ratio = displayValue_per_px, fixed at 96 DPI
	qreal ratio() const;

	/// true when system == None
	inline bool isPixelMode() const { return m_system == QET::UnitSystem::None; }

	/// Convert px to display unit (no-op in pixel mode)
	qreal toDisplay(qreal px) const;

	/// Convert display unit to px (no-op in pixel mode)
	qreal fromDisplay(qreal display) const;

	static QString unitName(QET::UnitSystem sys);
	static QString unitSuffix(QET::UnitSystem sys);

signals:
	void unitSystemChanged();

private:
	UnitConverter();
	UnitConverter(const UnitConverter &) = delete;
	~UnitConverter();

	inline static UnitConverter *m_instance = nullptr;

	QET::UnitSystem m_system;

	static constexpr qreal IN_PER_PX = 1.0 / 96.0;
	static constexpr qreal MM_PER_PX = 25.4 / 96.0;
	static constexpr qreal unitFactor[QET::UnitSystem::MaxUnit] = {
		1.0,                                // None
		IN_PER_PX,                          // Inch
		IN_PER_PX / 12.0,                   // Foot
		IN_PER_PX / 12.0 / 3.0 / 1760.0,    // Mile
		MM_PER_PX,                          // Millimeter
		MM_PER_PX / 10.0,                   // Centimeter
		MM_PER_PX / 1000.0,                 // Meter
		MM_PER_PX / 1e6,                    // Kilometer
		IN_PER_PX / 1e-6,                   // Microinch
		IN_PER_PX / 0.001,                  // Mil
		IN_PER_PX / 12.0 / 3.0,             // Yard
		MM_PER_PX / 1e-7,                   // Angstrom
		MM_PER_PX / 1e-6,                   // Nanometer
		MM_PER_PX / 1e-3,                   // Micron
		MM_PER_PX / 100.0,                  // Decimeter
		MM_PER_PX / 1e4,                    // Decameter
		MM_PER_PX / 1e5,                    // Hectometer
		MM_PER_PX / 1e12,                   // Gigameter
		MM_PER_PX / 1.495978707e14,         // Astro
		MM_PER_PX / 9.4607304725808e18,     // Lightyear
		MM_PER_PX / 3.08567758149137e19,    // Parsec
	};
};

#endif // UNITS_H
