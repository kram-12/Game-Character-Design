#include <iostream>
#include <string>
#include <list> // For storing all the abilities in a list
#include <vector> // For storing the characters
#include <map> // For storing the tag,layer,health and level of the player
#include <memory> // For using unique_ptr
#include <random> // For random number generation
#include <ctime> // For seeding the random number generator

#define MAX_ABILITIES 5
using namespace std;

// Base class for a generic game entity
class Entity {
public:
    string entityName;
    Entity() : entityName("Entity") {}
    Entity(const string& name) : entityName(name) {}

    virtual void displayInfo() {
        cout << "Entity Name: " << entityName << endl;
    }

    virtual ~Entity() = default;
};

// Base class for character abilities
class Abilities {
public:
    string abilityName;
    float abilityPower;
    Abilities() : abilityName("None"), abilityPower(0.0) {}
    Abilities(const string& name, float power) : abilityName(name), abilityPower(power) {}

    virtual ~Abilities() = default;
};

//Derived Class for CharacterType inherited from Enity class
class CharacterType : public Entity {
protected:
    map<string, string> tags;
    map<string, string> layers;

public:
    CharacterType() {
        tags["default"] = "None";
        layers["default"] = "Base";
    }

    CharacterType(const string& charName, const map<string, string>& charTags, const map<string, string>& charLayers)
        : Entity(charName), tags(charTags), layers(charLayers) {}

    virtual void displayInfo() override {
        cout << "Character: " << entityName << endl;
        for (const auto& pair : tags) {
            cout << "Tag: " << pair.first ;
        }
        for (const auto& pair : layers) {
            cout << " | Layer: " << pair.second ;
        }
        cout << endl;
    }
};

//A multiple inherited class for GameCharacter derived from CharacterType and Abilities
class GameCharacter : public CharacterType, public Abilities {
protected:
    list<Abilities> characterAbilities;
    // Add any additional members specific to GameCharacter if needed

public:
    map<string, float> characterHealth;
    map<string, int> characterLevel;

    GameCharacter()
        : CharacterType(), Abilities(), characterHealth{{"default", 100.0}}, characterLevel{{"default", 1}} {}

    GameCharacter(const string& charName, const map<string, string>& charTags, const map<string, string>& charLayers,
                  const string& abilityName, float abilityPower)
        : CharacterType(charName, charTags, charLayers), Abilities(abilityName, abilityPower) {
        characterHealth["default"] = 100.0;
        characterLevel["default"] = 1;
    }

    virtual void displayInfo() override {
        CharacterType::displayInfo();
        for (const auto& pair : characterHealth) {
            cout << "Health: " << pair.second ;
        }
        for (const auto& pair : characterLevel) {
            cout << " | Level: " << pair.second ;
        }
        cout << endl;
        cout << "Abilities: ";
        for (const auto& ability : characterAbilities) {
            cout << "[ " << ability.abilityName << " ] ";
        }
        cout << endl << endl;
    }

    void levelUp() {
        cout << entityName << " leveled up!" << endl << endl;
    }

    virtual float useAbility() = 0;

    void addAbility(const string& AbilityName, float AbilityPower) {
        if (characterAbilities.size() < MAX_ABILITIES) {
            Abilities temp;
            temp.abilityName=AbilityName;
            temp.abilityPower=AbilityPower;
            characterAbilities.push_back(temp);
            cout << entityName << " has learned a new ability: " << temp.abilityName << endl << endl;
        } else {
            cout << entityName << " cannot learn more abilities!" << endl << endl;
        }
    }
};

/* Derived classes for specialized characters inheriting from GameCharacter */

//Specific character of type Mage
class Mage : public GameCharacter {
public:
    Mage() : GameCharacter("DefaultMage", {{"default", "DefaultMage"}}, {{"default", "DefaultMage"}}, "DefaultAbility", 0.0) {}
    Mage(const string& charName, const string& charTag, const string& charLayer)
        : GameCharacter(charName, {{charTag, charName}}, {{charTag, charLayer}}, "DefaultAbility", 0.0) {}

    float useAbility() override {
        if (characterAbilities.empty()) {
            cout << entityName << " has no abilities!" << endl << endl;
            return 0.0;
        }

        // Seed the random number generator
        srand(static_cast<unsigned int>(time(nullptr)));

        // Generate a random index within the range of the abilities list
        int randomIndex = rand() % characterAbilities.size();

        // Iterate to the random index
        auto it = characterAbilities.begin();
        advance(it, randomIndex);

        cout << entityName << " uses ability: " << it->abilityName << endl << endl;

        return it->abilityPower;
    }
};

//Specific character of type Rogue
class Rogue : public GameCharacter {
public:
    Rogue() : GameCharacter("DefaultRogue", {{"default", "DefaultRogue"}}, {{"default", "DefaultRogue"}}, "DefaultAbility", 0.0) {}
    Rogue(const string& charName, const string& charTag, const string& charLayer)
        : GameCharacter(charName, {{charTag, charName}}, {{charTag, charLayer}}, "DefaultAbility", 0.0) {}

    float useAbility() override {
        if (characterAbilities.empty()) {
            cout << entityName << " has no abilities!" << endl << endl;
            return 0.0;
        }

        // Seed the random number generator
        srand(static_cast<unsigned int>(time(nullptr)));

        // Generate a random index within the range of the abilities list
        int randomIndex = rand() % characterAbilities.size();

        // Iterate to the random index
        auto it = characterAbilities.begin();
        advance(it, randomIndex);

        cout << entityName << " uses ability: " << it->abilityName << endl << endl;

        return it->abilityPower;
    }
};

// Function template to make characters fight with each other
template<typename T1, typename T2>
void fightPlayers(T1& player1, T2& player2) {
    cout << "\nFight between " << player1.entityName << " and " << player2.entityName << " begins!" << endl << endl;
    player1.displayInfo();
    player2.displayInfo();

    // This is to make changes to the level and health of players 
    float pl1_power = player1.useAbility();
    float pl2_power = player2.useAbility();

    // Access health and level using the characterHealth and characterLevel maps
    for (auto& pair : player1.characterHealth) {
        pair.second -= pl2_power;
    }
    for (auto& pair : player2.characterHealth) {
        pair.second -= pl1_power;
    }

    for (auto& pair : player1.characterLevel) {
        if (pl1_power > pl2_power) {
            pair.second++;
            if (player1.characterHealth[pair.first] >= 90.0)
                player1.characterHealth[pair.first] = 100.0;
            else
                player1.characterHealth[pair.first] += 10.0;
            player1.levelUp();
        } else if (pl1_power < pl2_power) {
            player2.characterLevel[pair.first]++;
            if (player2.characterHealth[pair.first] >= 90.0)
                player2.characterHealth[pair.first] = 100.0;
            else
                player2.characterHealth[pair.first] += 10.0;
            player2.levelUp();
        } else {
            pair.second++;
            player1.characterHealth[pair.first] += 10.0;
            player2.characterHealth[pair.first] += 10.0;
            player1.levelUp();
            player2.levelUp();
        }
    }

    player1.displayInfo();
    player2.displayInfo();
    
    cout << "Fight concluded!" << endl << endl;
}

int main() {
    vector<unique_ptr<GameCharacter>> characters;

    unique_ptr<Mage> gandalf = make_unique<Mage>("Gandalf", "Wizard", "Player1");
    unique_ptr<Rogue> legolas = make_unique<Rogue>("Legolas", "Archer", "Player2");

    gandalf->addAbility("Fireball", 25.0);
    gandalf->addAbility("Teleport", 30.0);
    legolas->addAbility("Stealth Attack", 20.0);
    legolas->addAbility("Arrow Rain", 28.0);
    legolas->addAbility("Eagle Eye", 32.0);

    gandalf->displayInfo();
    legolas->displayInfo();

    characters.push_back(move(gandalf));
    characters.push_back(move(legolas));

    if (characters.size() >= 2) {
        auto it = characters.begin();
        auto& firstCharacter = *it;
        ++it;
        auto& secondCharacter = *it;

        if (firstCharacter && secondCharacter) {
            fightPlayers(*firstCharacter, *secondCharacter);
        }
    }
    return 0;
}