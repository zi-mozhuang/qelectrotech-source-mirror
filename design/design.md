实现可选的绘图单位。
新的程序可以在设置面板设置单位制。启用新单位后，原本的数据类型px不变，但显示的值由px换算为新单位显示，写入的新单位值换算为px保存。

方案要求：
- 尽可能避免改动原项目的内容。
- 尽可能利用原项目已有的函数。
- 方案逐步进行，每步改动后能即时测试。
- 完成方案后，检查每组改动的共同点，思考能否减少修改对象。

设计细则：

1. 可选的单位列表：
namespace QET {
    enum UnitSystem {
        None       = 0,    // Pixel
        Inch       = 1,    // Inch: 1 inch = 25.4 mm = 96 px
        Foot       = 2,    // Foot: 1 foot = 12 inch
        Mile       = 3,    // Mile: 1 mile = 1760 yard
        Millimeter = 4,    // Millimeter
        Centimeter = 5,    // Centimeter: 1 cm = 10 mm
        Meter      = 6,    // Meter: 1 m = 1000 mm
        Kilometer  = 7,    // Kilometer: 1 km = 10^6 mm
        Microinch  = 8,    // Microinch: 1 µin = 10^-6 inch
        Mil        = 9,    // Mil: 1 mil = 0.001 inch
        Yard       = 10,   // Yard: 1 yard = 3 feet
        Angstrom   = 11,   // Angstrom: 1 Å = 10^-10 m
        Nanometer  = 12,   // Nanometer: 1 nm = 10^-9 m
        Micron     = 13,   // Micron: 1 µ = 10^-6 m
        Decimeter  = 14,   // Decimeter: 1 dm = 100 mm
        Decameter  = 15,   // Decameter: 1 dam = 10 m
        Hectometer = 16,   // Hectometer: 1 hm = 100 m
        Gigameter  = 17,   // Gigameter: 1 Gm = 10^9 m
        Astro      = 18,   // Astronomical: 1 AU = 149597870700 m
        Lightyear  = 19,   // Lightyear: 1 ly = 9.4607×10^15 m
        Parsec     = 20,   // Parsec: 1 pc = 3.0857×10^16 m
        Max        = 21
    };
}

2. 确定不同单位对px的换算关系，建立换算因子表。
    const int DPI = 96; 
    const qreal IN_PER_PX = 1.0 / DPI; // 1 px = 1/96 inch
    const qreal MM_PER_PX = 25.4 / DPI; // 1 px = 25.4/96 mm
    const qreal unitFactor[QET::UnitSystem::Max] = {
        [QET::UnitSystem::None]       = 1.0,
        [QET::UnitSystem::Inch]       = IN_PER_PX,
        [QET::UnitSystem::Foot]       = IN_PER_PX / 12.0,
        [QET::UnitSystem::Mile]       = IN_PER_PX / 12.0 / 3.0 / 1760.0,
        [QET::UnitSystem::Millimeter] = MM_PER_PX,
        [QET::UnitSystem::Centimeter] = MM_PER_PX / 10.0,
        [QET::UnitSystem::Meter]      = MM_PER_PX / 1000.0,
        [QET::UnitSystem::Kilometer]  = MM_PER_PX / 1e6,
        [QET::UnitSystem::Microinch]  = IN_PER_PX / 1e-6,
        [QET::UnitSystem::Mil]        = IN_PER_PX / 0.001,
        [QET::UnitSystem::Yard]       = IN_PER_PX / 12.0 / 3.0,
        [QET::UnitSystem::Angstrom]   = MM_PER_PX / 1e-7,
        [QET::UnitSystem::Nanometer]  = MM_PER_PX / 1e-6,
        [QET::UnitSystem::Micron]     = MM_PER_PX / 1e-3,
        [QET::UnitSystem::Decimeter]  = MM_PER_PX / 100.0,
        [QET::UnitSystem::Decameter]  = MM_PER_PX / 1e4,
        [QET::UnitSystem::Hectometer] = MM_PER_PX / 1e5,
        [QET::UnitSystem::Gigameter]  = MM_PER_PX / 1e12,
        [QET::UnitSystem::Astro]      = MM_PER_PX / 1.4959e14,
        [QET::UnitSystem::Lightyear]  = MM_PER_PX / 9.4607e18,
        [QET::UnitSystem::Parsec]     = MM_PER_PX / 3.0857e19,
    };

3. 原有的QGraphicsView 坐标转换QGraphicsScene 坐标，转换前QGraphicsView 坐标乘了缩放平移矩阵，类似AB=C。现在将单位换算因子作为D，放在乘以缩放平移矩阵前，类似ADB=C。反向同理。

4. 栅格、吸附点从原有的QGraphicsView转换到QGraphicsScene时，不能简单乘以单位换算因子D。应该理解，默认设置下，px单位下格点间距为10px或10逻辑单位，mm单位下格点间距应为37.7953px或逻辑单位。不要采用取整设计,以获得精确的mm显示和输入。