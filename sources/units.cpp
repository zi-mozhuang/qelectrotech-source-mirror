#include "units.h"

UnitConverter *UnitConverter::instance() {
	if (!m_instance) m_instance = new UnitConverter();
	return m_instance;
}

void UnitConverter::dropInstance() {
	delete m_instance; m_instance = nullptr;
}

UnitConverter::UnitConverter()
	: QObject(nullptr)
	, m_system(QET::UnitSystem::None)
{}

UnitConverter::~UnitConverter() {}

QET::UnitSystem UnitConverter::system() const { return m_system; }

void UnitConverter::setSystem(QET::UnitSystem sys)
{
	sys = qBound(QET::UnitSystem::None, sys, static_cast<QET::UnitSystem>(QET::UnitSystem::Parsec));
	if (m_system != sys) {
		m_system = sys;
		emit unitSystemChanged();
	}
}

qreal UnitConverter::ratio() const {
	return unitFactor[m_system];
}

qreal UnitConverter::toDisplay(qreal px) const {
	return isPixelMode() ? px : px * unitFactor[m_system];
}

qreal UnitConverter::fromDisplay(qreal display) const {
	return isPixelMode() ? display : display / unitFactor[m_system];
}

QString UnitConverter::unitName(QET::UnitSystem sys)
{
	switch (sys) {
		case QET::None:       return QObject::tr("Pixel (px)");
		case QET::Inch:       return QObject::tr("Pouce (in)");
		case QET::Foot:       return QObject::tr("Pied (ft)");
		case QET::Mile:       return QObject::tr("Mille (mi)");
		case QET::Millimeter: return QObject::tr("Millimètre (mm)");
		case QET::Centimeter: return QObject::tr("Centimètre (cm)");
		case QET::Meter:      return QObject::tr("Mètre (m)");
		case QET::Kilometer:  return QObject::tr("Kilomètre (km)");
		case QET::Microinch:  return QObject::tr("Micro-pouce (µin)");
		case QET::Mil:        return QObject::tr("Millième de pouce (mil)");
		case QET::Yard:       return QObject::tr("Yard (yd)");
		case QET::Angstrom:   return QObject::tr("Angström (Å)");
		case QET::Nanometer:  return QObject::tr("Nanomètre (nm)");
		case QET::Micron:     return QObject::tr("Micron (µm)");
		case QET::Decimeter:  return QObject::tr("Décimètre (dm)");
		case QET::Decameter:  return QObject::tr("Décamètre (dam)");
		case QET::Hectometer: return QObject::tr("Hectomètre (hm)");
		case QET::Gigameter:  return QObject::tr("Gigamètre (Gm)");
		case QET::Astro:      return QObject::tr("Unité astronomique (AU)");
		case QET::Lightyear:  return QObject::tr("Année-lumière (ly)");
		case QET::Parsec:     return QObject::tr("Parsec (pc)");
		default:              return QObject::tr("Pixel (px)");
	}
}

QString UnitConverter::unitSuffix(QET::UnitSystem sys)
{
	switch (sys) {
		case QET::None:       return QObject::tr(" px");
		case QET::Inch:       return QObject::tr(" in");
		case QET::Foot:       return QObject::tr(" ft");
		case QET::Mile:       return QObject::tr(" mi");
		case QET::Millimeter: return QObject::tr(" mm");
		case QET::Centimeter: return QObject::tr(" cm");
		case QET::Meter:      return QObject::tr(" m");
		case QET::Kilometer:  return QObject::tr(" km");
		case QET::Microinch:  return QObject::tr(" µin");
		case QET::Mil:        return QObject::tr(" mil");
		case QET::Yard:       return QObject::tr(" yd");
		case QET::Angstrom:   return QObject::tr(" Å");
		case QET::Nanometer:  return QObject::tr(" nm");
		case QET::Micron:     return QObject::tr(" µm");
		case QET::Decimeter:  return QObject::tr(" dm");
		case QET::Decameter:  return QObject::tr(" dam");
		case QET::Hectometer: return QObject::tr(" hm");
		case QET::Gigameter:  return QObject::tr(" Gm");
		case QET::Astro:      return QObject::tr(" AU");
		case QET::Lightyear:  return QObject::tr(" ly");
		case QET::Parsec:     return QObject::tr(" pc");
		default:              return QObject::tr(" px");
	}
}
