#include "../../../includes.h"

const std::map<eHero, std::string> heroNames = {
		{ eHero::HERO_REAPER, enc("Reaper") },
		{ eHero::HERO_TRACER, enc("Tracer") },
		{ eHero::HERO_MERCY, enc("Mercy") },
		{ eHero::HERO_HANZO, enc("Hanzo") },
		{ eHero::HERO_TORBJORN, enc("Torbjorn") },
		{ eHero::HERO_REINHARDT, enc("Reinhardt") },
		{ eHero::HERO_PHARAH, enc("Pharah") },
		{ eHero::HERO_WINSTON, enc("Winston") },
		{ eHero::HERO_WIDOWMAKER, enc("Widowmaker") },
		{ eHero::HERO_BASTION, enc("Bastion") },
		{ eHero::HERO_SYMMETRA, enc("Symmetra") },
		{ eHero::HERO_ZENYATTA, enc("Zenyatta") },
		{ eHero::HERO_GENJI, enc("Genji") },
		{ eHero::HERO_ROADHOG, enc("Roadhog") },
		{ eHero::HERO_CASSIDY, enc("Cassidy") },
		{ eHero::HERO_JUNKRAT, enc("Junkrat") },
		{ eHero::HERO_ZARYA, enc("Zarya") },
		{ eHero::HERO_SOLDIER76, enc("Soldier 76") },
		{ eHero::HERO_LUCIO, enc("Lucio") },
		{ eHero::HERO_DVA, enc("D.Va") },
		{ eHero::HERO_MEI, enc("Mei") },
		{ eHero::HERO_ANA, enc("Ana") },
		{ eHero::HERO_SOMBRA, enc("Sombra") },
		{ eHero::HERO_ORISA, enc("Orisa") },
		{ eHero::HERO_DOOMFIST, enc("Doomfist") },
		{ eHero::HERO_MOIRA, enc("Moira") },
		{ eHero::HERO_BRIGITTE, enc("Brigitte") },
		{ eHero::HERO_WRECKINGBALL, enc("Wrecking Ball") },
		{ eHero::HERO_ASHE, enc("Ashe") },
		{ eHero::HERO_BAPTISTE, enc("Baptiste") },
		{ eHero::HERO_SIGMA, enc("Sigma") },
		{ eHero::HERO_ECHO, enc("Echo") },
		{ eHero::HERO_KIRIKO, enc("Kiriko") },
		{ eHero::HERO_RAMATTRA, enc("Ramattra") },
		{ eHero::HERO_LIFEWEAVER, enc("Lifeweaver") },
		{ eHero::HERO_SOJOURN, enc("Sojourn") },
		{ eHero::HERO_ILLARI, enc("Illari") },
		{ eHero::HERO_JUNKERQUEEN, enc("Junker Queen") },
		{ eHero::HERO_VENTURE, enc("Venture") },
		{ eHero::HERO_MAUGA, enc("Mauga") },
		{ eHero::HERO_JUNO, enc("Juno") },
		{ eHero::HERO_HAZARD, enc("Hazard") },
		{ eHero::HERO_TRAININGBOT1, enc("Friendly Bot") },
		{ eHero::HERO_TRAININGBOT2, enc("Training Bot") },
		{ eHero::HERO_TRAININGBOT3, enc("Heavy Bot") },
		{ eHero::HERO_TRAININGBOT4, enc("Training Bot") },
		{ eHero::HERO_TRAININGBOT5, enc("Rocket Bot") },
		{ eHero::HERO_TRAININGBOT6, enc("Sniper Bot") },
		{ eHero::HERO_TRAININGBOT7, enc("Friendly Heavy Bot") },
};

uint32_t HeroComponent::SkinID() const
{
	return RPM<uint32_t>(GetComponentBase() + Offsets::HeroIdentifier::SkinID);
}

eHero HeroComponent::HeroID() const
{
	return RPM<eHero>(GetComponentBase() + Offsets::HeroIdentifier::HeroID);
}

std::string HeroComponent::GetHeroName() const {
	const auto heroNameIter = heroNames.find(HeroID());
	if (heroNameIter != heroNames.end()) {
		return heroNameIter->second;
	}
	else {
		return enc("None");
	}
}