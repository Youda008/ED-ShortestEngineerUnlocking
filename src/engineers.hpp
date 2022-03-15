//======================================================================================================================
//  Project: ShortestEngineerUnlocking
//  Author:  Youda008
//======================================================================================================================

#ifndef ENGINEERS_INCLUDED
#define ENGINEERS_INCLUDED


#include "modules.hpp"

#include <vector>


//======================================================================================================================

enum EngineerIdx : uint
{
	None = 0,

	FelicityFarseer,
	ElviraMartuuk,
	MarcoQwent,
	IshmaelPalin,
	ChloeSedesi,
	TheDweller,
	LeiCheung,
	TodMcQuinn,
	SeleneJean,
	BillTurner,
	DidiVatermann,
	LizRyder,
	HeraTani,
	JuriIshmaak,
	BrooTarquin,
	ZacariahNemo,
	LoriJameson,
	RamTah,
	TianaFortune,
	BrisDekker,
	TheSarge,
	MarshaHicks,
	MelBrandon,
	PetraOlmanova,
	EtienneDorn,

	_EndOfEnum
};

static constexpr EngineerIdx firstEngineerIdx = EngineerIdx( 1 );
static constexpr EngineerIdx lastEngineerIdx = EngineerIdx( EngineerIdx::_EndOfEnum - 1 );
static constexpr size_t numOfEngineers = lastEngineerIdx - firstEngineerIdx + 1;

static const char * EngineerNameStr [] =
{
	"<none>",
	"Felicity Farseer",
	"Elvira Martuuk",
	"Marco Qwent",
	"Ishmael Palin",
	"Chloe Sedesi",
	"The Dweller",
	"Lei Cheung",
	"Tod McQuinn",
	"Selene Jean",
	"Bill Turner",
	"Didi Vatermann",
	"Liz Ryder",
	"Hera Tani",
	"Juri Ishmaak",
	"Broo Tarquin",
	"Zacariah Nemo",
	"Lori Jameson",
	"Ram Tah",
	"Tiana Fortune",
	"Bris Dekker",
	"The Sarge",
	"Marsha Hicks",
	"Mel Brandon",
	"Petra Olmanova",
	"Etienne Dorn",
};

static_assert( size_t(EngineerIdx::_EndOfEnum) == std::size(EngineerNameStr), "string table and enum do not match" );


//======================================================================================================================

struct EngineerInfo
{
	EngineerIdx requiredEngineer;
	std::vector< Modification > modifications;
};

struct Engineer
{
	EngineerIdx idx;
	EngineerInfo info;
};

static const Engineer _engineers [] =
{
	{ None, { None, {} } },  // first engineer has index 1 so we have to put a placeholder here

	{
		FelicityFarseer,
		{
			None,
			{
				{ 3, ModuleType::DetailedSurfaceScanner },
				{ 5, ModuleType::FrameShiftDrive },
				{ 1, ModuleType::FrameShiftDriveInterdictor },
				{ 1, ModuleType::PowerPlant },
				{ 3, ModuleType::Sensors },
				{ 1, ModuleType::ShieldBooster },
				{ 3, ModuleType::Thrusters },
			},
		}
	},

	{
		ElviraMartuuk,
		{
			None,
			{
				{ 5, ModuleType::FrameShiftDrive },
				{ 3, ModuleType::ShieldGenerator },
				{ 2, ModuleType::Thrusters },
				{ 1, ModuleType::ShieldCellBank },
			},
		}
	},

	{
		MarcoQwent,
		{
			ElviraMartuuk,
			{
				{ 3, ModuleType::PowerDistributor },
				{ 4, ModuleType::PowerPlant },
			},
		}
	},

	{
		IshmaelPalin,
		{
			MarcoQwent,
			{
				{ 3, ModuleType::FrameShiftDrive },
				{ 5, ModuleType::Thrusters },
			},
		}
	},

	{
		ChloeSedesi,
		{
			MarcoQwent,
			{
				{ 3, ModuleType::FrameShiftDrive },
				{ 5, ModuleType::Thrusters },
			},
		}
	},

	{
		TheDweller,
		{
			None,
			{
				{ 3, ModuleType::BeamLaser },
				{ 3, ModuleType::BurstLaser },
				{ 4, ModuleType::PulseLaser },
				{ 5, ModuleType::PowerDistributor },
			},
		}
	},

	{
		LeiCheung,
		{
			TheDweller,
			{
				{ 5, ModuleType::DetailedSurfaceScanner },
				{ 5, ModuleType::Sensors },
				{ 3, ModuleType::ShieldBooster },
				{ 5, ModuleType::ShieldGenerator },
			},
		}
	},

	{
		TodMcQuinn,
		{
			None,
			{
				{ 2, ModuleType::Cannon },
				{ 3, ModuleType::FragmentCannon },
				{ 5, ModuleType::MultiCannon },
				{ 5, ModuleType::RailGun },
			},
		}
	},

	{
		SeleneJean,
		{
			TodMcQuinn,
			{
				{ 5, ModuleType::Armour },
				{ 5, ModuleType::HullReinforcementPackage },
			},
		}
	},

	{
		BillTurner,
		{
			SeleneJean,
			{
				{ 3, ModuleType::AutoFieldMaintenanceUnit },
				{ 5, ModuleType::DetailedSurfaceScanner },
				{ 3, ModuleType::FrameShiftWakeScanner },
				{ 3, ModuleType::FuelScoop },
				{ 3, ModuleType::KillWarrantScanner },
				{ 3, ModuleType::LifeSupport },
				{ 3, ModuleType::ManifestScanner },
				{ 5, ModuleType::PlasmaAccelerator },
				{ 3, ModuleType::Refinery },
				{ 5, ModuleType::Sensors },
			},
		}
	},

	{
		DidiVatermann,
		{
			SeleneJean,
			{
				{ 5, ModuleType::ShieldBooster },
				{ 3, ModuleType::ShieldGenerator },
			},
		}
	},

	{
		LizRyder,
		{
			None,
			{
				{ 3, ModuleType::MineLauncher },
				{ 5, ModuleType::MissileRack },
				{ 5, ModuleType::SeekerMissileRack },
				{ 5, ModuleType::TorpedoPylon },
				{ 1, ModuleType::HullReinforcementPackage },
				{ 1, ModuleType::Armour },
			},
		}
	},

	{
		HeraTani,
		{
			LizRyder,
			{
				{ 5, ModuleType::DetailedSurfaceScanner },
				{ 3, ModuleType::PowerDistributor },
				{ 5, ModuleType::PowerPlant },
				{ 3, ModuleType::Sensors },
			},
		}
	},

	{
		JuriIshmaak,
		{
			FelicityFarseer,
			{
				{ 5, ModuleType::DetailedSurfaceScanner },
				{ 3, ModuleType::FrameShiftWakeScanner },
				{ 3, ModuleType::KillWarrantScanner },
				{ 3, ModuleType::ManifestScanner },
				{ 5, ModuleType::MineLauncher },
				{ 3, ModuleType::MissileRack },
				{ 3, ModuleType::SeekerMissileRack },
				{ 5, ModuleType::Sensors },
				{ 3, ModuleType::TorpedoPylon },
			},
		}
	},

	{
		BrooTarquin,
		{
			HeraTani,
			{
				{ 5, ModuleType::BeamLaser },
				{ 5, ModuleType::BurstLaser },
				{ 5, ModuleType::PulseLaser },
			},
		}
	},

	{
		ZacariahNemo,
		{
			ElviraMartuuk,
			{
				{ 5, ModuleType::FragmentCannon },
				{ 3, ModuleType::MultiCannon },
				{ 2, ModuleType::PlasmaAccelerator },
			},
		}
	},

	{
		LoriJameson,
		{
			MarcoQwent,
			{
				{ 4, ModuleType::AutoFieldMaintenanceUnit },
				{ 5, ModuleType::DetailedSurfaceScanner },
				{ 3, ModuleType::FrameShiftWakeScanner },
				{ 4, ModuleType::FuelScoop },
				{ 3, ModuleType::KillWarrantScanner },
				{ 4, ModuleType::LifeSupport },
				{ 3, ModuleType::ManifestScanner },
				{ 4, ModuleType::Refinery },
				{ 5, ModuleType::Sensors },
				{ 3, ModuleType::ShieldCellBank },
			},
		}
	},

	{
		RamTah,
		{
			LeiCheung,
			{
				{ 5, ModuleType::ChaffLauncher },
				{ 4, ModuleType::CollectorLimpetController },
				{ 5, ModuleType::ElectronicCountermeasure },
				{ 4, ModuleType::FuelTransferLimpetController },
				{ 3, ModuleType::HatchBreakerLimpetController },
				{ 5, ModuleType::HeatSinkLauncher },
				{ 5, ModuleType::PointDefence },
				{ 4, ModuleType::ProspectorLimpetController },
			},
		}
	},

	{
		TianaFortune,
		{
			HeraTani,
			{
				{ 5, ModuleType::ManifestScanner },
				{ 5, ModuleType::CollectorLimpetController },
				{ 3, ModuleType::DetailedSurfaceScanner },
				{ 3, ModuleType::FrameShiftDriveInterdictor },
				{ 5, ModuleType::FrameShiftWakeScanner },
				{ 5, ModuleType::FuelTransferLimpetController },
				{ 5, ModuleType::HatchBreakerLimpetController },
				{ 5, ModuleType::KillWarrantScanner },
				{ 5, ModuleType::ProspectorLimpetController },
				{ 5, ModuleType::Sensors },
			},
		}
	},

	{
		BrisDekker,
		{
			JuriIshmaak,
			{
				{ 3, ModuleType::FrameShiftDrive },
				{ 4, ModuleType::FrameShiftDriveInterdictor },
			},
		}
	},

	{
		TheSarge,
		{
			JuriIshmaak,
			{
				{ 5, ModuleType::Cannon },
				{ 5, ModuleType::CollectorLimpetController },
				{ 5, ModuleType::FuelTransferLimpetController },
				{ 5, ModuleType::HatchBreakerLimpetController },
				{ 5, ModuleType::ProspectorLimpetController },
				{ 3, ModuleType::RailGun },
			},
		}
	},

	{
		MarshaHicks,
		{
			TheDweller,
			{
				{ 5, ModuleType::Cannon },
				{ 5, ModuleType::CollectorLimpetController },
				{ 5, ModuleType::FragmentCannon },
				{ 5, ModuleType::FuelScoop },
				{ 5, ModuleType::FuelTransferLimpetController },
				{ 5, ModuleType::HatchBreakerLimpetController },
				{ 5, ModuleType::MultiCannon },
				{ 5, ModuleType::ProspectorLimpetController },
				{ 5, ModuleType::Refinery },
			},
		}
	},

	{
		MelBrandon,
		{
			ElviraMartuuk,
			{
				{ 5, ModuleType::BeamLaser },
				{ 5, ModuleType::BurstLaser },
				{ 5, ModuleType::PulseLaser },
				{ 5, ModuleType::ShieldGenerator },
				{ 5, ModuleType::Thrusters },
				{ 5, ModuleType::ShieldBooster },
				{ 5, ModuleType::FrameShiftDrive },
				{ 5, ModuleType::FrameShiftDriveInterdictor },
				{ 4, ModuleType::ShieldCellBank },
			},
		}
	},

	{
		PetraOlmanova,
		{
			TodMcQuinn,
			{
				{ 5, ModuleType::Armour },
				{ 5, ModuleType::AutoFieldMaintenanceUnit },
				{ 5, ModuleType::ChaffLauncher },
				{ 5, ModuleType::ElectronicCountermeasure },
				{ 5, ModuleType::HeatSinkLauncher },
				{ 5, ModuleType::HullReinforcementPackage },
				{ 5, ModuleType::MineLauncher },
				{ 5, ModuleType::MissileRack },
				{ 5, ModuleType::PointDefence },
				{ 5, ModuleType::SeekerMissileRack },
				{ 5, ModuleType::TorpedoPylon },
			},
		}
	},

	{
		EtienneDorn,
		{
			LizRyder,
			{
				{ 5, ModuleType::DetailedSurfaceScanner },
				{ 5, ModuleType::FrameShiftWakeScanner },
				{ 5, ModuleType::KillWarrantScanner },
				{ 5, ModuleType::LifeSupport },
				{ 5, ModuleType::ManifestScanner },
				{ 5, ModuleType::PlasmaAccelerator },
				{ 5, ModuleType::PowerDistributor },
				{ 5, ModuleType::PowerPlant },
				{ 5, ModuleType::Sensors },
				{ 5, ModuleType::RailGun },
			},
		}
	},
};


//----------------------------------------------------------------------------------------------------------------------
//  Thanks GCC for still not supporting designated array initializers
//  and thanks mingw64-clang for not linking standard library stuff properly.

static EngineerInfo engineers [ std::size(_engineers) ];

bool initializeEngineers()
{
	for (const auto & engineer : _engineers)
	{
		engineers[ engineer.idx ] = engineer.info;
	}
	return true;
}
// This will cause the function to be called right on the application startup, even before main is entered.
static const bool engineerInitializer = initializeEngineers();


#endif // ENGINEERS_INCLUDED
