//======================================================================================================================
//  Project: ShortestEngineerUnlocking
//  Author:  Youda008
//======================================================================================================================

#ifndef MODULES_INCLUDED
#define MODULES_INCLUDED


#include <iterator>  // size_t, std::size

using uint = unsigned int;
using ushort = unsigned short;


enum class ModuleType : ushort
{
	Thrusters,
	PowerPlant,
	PowerDistributor,
	FrameShiftDrive,
	LifeSupport,

	Armour,
	HullReinforcementPackage,
	ShieldGenerator,
	ShieldCellBank,
	ShieldBooster,

	Sensors,
	DetailedSurfaceScanner,
	FrameShiftWakeScanner,
	KillWarrantScanner,
	ManifestScanner,

	CollectorLimpetController,
	FuelTransferLimpetController,
	HatchBreakerLimpetController,
	ProspectorLimpetController,

	FuelScoop,
	Refinery,
	FrameShiftDriveInterdictor,
	AutoFieldMaintenanceUnit,

	PointDefence,
	ElectronicCountermeasure,
	ChaffLauncher,
	HeatSinkLauncher,

	BeamLaser,
	BurstLaser,
	PulseLaser,
	MultiCannon,
	Cannon,
	FragmentCannon,
	RailGun,
	PlasmaAccelerator,
	MissileRack,
	SeekerMissileRack,
	TorpedoPylon,
	MineLauncher,

	_EndOfEnum
};

static constexpr ModuleType firstModule = ModuleType( 0 );
static constexpr ModuleType lastModule = ModuleType( size_t(ModuleType::_EndOfEnum) - 1 );

static const char * const ModuleTypeStr [] =
{
	"Thrusters",
	"Power Plant",
	"Power Distributor",
	"Frame Shift Drive",
	"Life Support",

	"Armour",
	"Hull Reinforcement Package",
	"Shield Generator",
	"Shield Cell Bank",
	"Shield Booster",

	"Sensors",
	"Detailed Surface Scanner",
	"Frame Shift Wake Scanner",
	"Kill Warrant Scanner",
	"Manifest Scanner",

	"Collector Limpet Controller",
	"Fuel Transfer Limpet Controller",
	"Hatch Breaker Limpet Controller",
	"Prospector Limpet Controller",

	"Fuel Scoop",
	"Refinery",
	"Frame Shift Drive Interdictor",
	"Auto Field-Maintenance Unit",

	"Point Defence",
	"Electronic Countermeasure",
	"Chaff Launcher",
	"Heat Sink Launcher",

	"Beam Laser",
	"Burst Laser",
	"Pulse Laser",
	"Multi-cannon",
	"Cannon",
	"Fragment Cannon",
	"Rail Gun",
	"Plasma Accelerator",
	"Missile Rack",
	"Seeker Missile Rack",
	"Torpedo Pylon",
	"Mine Launcher",
};

static_assert( size_t(ModuleType::_EndOfEnum) == std::size(ModuleTypeStr), "string table and enum do not match" );


//----------------------------------------------------------------------------------------------------------------------

using grade_t = ushort;

struct Modification
{
	grade_t grade;
	ModuleType module;

	Modification() : grade(0) {}
	Modification( grade_t g, ModuleType m ) : grade(g), module(m) {}

	bool valid() const { return grade != 0; }  // used for indicating errors

	// i don't know why this just can't be generated automatically
	bool operator==( const Modification & other )
	{
		return this->module == other.module && this->grade == other.grade;
	}
};


#endif // MODULES_INCLUDED
