//
// Created by paolo on 08/01/21.
//

#include <cmath>
#include <iostream>
#include <Constants.h>
#include "../include/GameMap.h"

GameMap::GameMap(std::string filename)
:name(filename)
{
    loaded_ = false;
    width_ = 0;
    height_ = 0;
}

GameMap::~GameMap() {
    for (int i = 0; i < width_; i++)
    {
        delete[] graphicsLayer_[i];
        delete[] backgroundLayer_[i];
    }
}

void GameMap::loadMap(ECS::World* world) {
    tmx::Map map;
    if(map.load(name))
    {
        // Get Map sizes
        loadMapBasicInfo(map.getTileCount());

        // Load properties
        loadProperties(map.getProperties());

        // Load layers
        std::set<unsigned int> usedTiles = loadLayers(map.getLayers(), world);

        // Load tileset
        loadMapTiles(const_cast<std::vector<tmx::Tileset> &>(map.getTilesets()), usedTiles);
    } else {
        throw "Cannot load map from " + name;
    }

    this->loaded_ = true;
}

void GameMap::loadMapBasicInfo(const tmx::Vector2u &orientation) {
    width_ = orientation.x;
    height_ = orientation.y;

    graphicsLayer_ = new unsigned int*[width_];
    backgroundLayer_ = new unsigned int*[width_];
    for (int i = 0; i < width_; i++)
    {
        graphicsLayer_[i] = new unsigned int[height_];
        backgroundLayer_[i] = new unsigned int[height_];
    }
}

void GameMap::loadProperties(const std::vector<tmx::Property> properties) {
    Vector2 marioSpawn;
    for (const tmx::Property& property : properties)
    {
        if (property.getName() == "spawn_mario_x") {
            marioSpawn.x = property.getIntValue();

        } else if (property.getName() == "spawn_mario_y") {
            marioSpawn.y = property.getIntValue();
        }
    }

    spawnPositionP1_ = marioSpawn;
    spawnPostionP2_ = Vector2{marioSpawn.x - 2, marioSpawn.y};
}

std::set<unsigned int> GameMap::loadLayers(const std::vector<tmx::Layer::Ptr>& layers, ECS::World* world) {
    std::set<unsigned int> usedTilesSet;

    for (const auto& layer : layers)
    {
        if(layer->getType() == tmx::Layer::Type::Object)
        {
            std::string layerName = layer->getName();
            const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
            const auto& objects = objectLayer.getObjects();

            for (const auto& object : objects)
            {
                std::vector<tmx::Property> properties = object.getProperties();
                tmx::FloatRect AABB = object.getAABB();
                ECS::Entity* ent = world->create();

                loadTileEntity(ent, AABB, properties, layerName);
            }
        }
        else if(layer->getType() == tmx::Layer::Type::Tile)
        {
            unsigned int** mapToLoad;
            if (layer->getName() == "background")
            {
                mapToLoad = backgroundLayer_;
            } else if (layer->getName() == "graphics")
            {
                mapToLoad = graphicsLayer_;
            }

            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto & tiles = tileLayer.getTiles();
            for (int j = 0; j < height_; j++)
            {
                for (int i = 0; i < width_; i++)
                {
                    unsigned int value = tiles.at(i + j * width_).ID;
                    mapToLoad[i][j] = value;
                    usedTilesSet.insert(value);
                }
            }
        }
    }

    return usedTilesSet;
}

Texture2D GameMap::getTexture(const std::string& path, tmx::Vector2u tilePosition, tmx::Vector2u tileSize) {
    Image image = LoadImage(path.c_str());
    ImageCrop(&image, (Rectangle){
        static_cast<float>(tilePosition.x - 16.f),
        static_cast<float>(tilePosition.y),
        static_cast<float>(tileSize.x),
        static_cast<float>(tileSize.y)});
    ImageResize(&image, 32, 32);

    Texture2D texture2D = LoadTextureFromImage(image);
    UnloadImage(image);

    return texture2D;
}

void GameMap::loadMapTiles(std::vector<tmx::Tileset> &tileset, const std::set<unsigned int>& usedTiles) {
    const auto& tiles = tileset.at(0).getTiles();
    for(const auto& tile : tiles)
    {
        if (usedTiles.count(tile.ID))
        {
            Texture2D texture2D = getTexture(tile.imagePath, tile.imagePosition, tile.imageSize);
            TileTexture pTileTexture = static_cast<TileTexture>(malloc(sizeof(TileTexture)));
            pTileTexture->texture = texture2D;
            pTileTexture->id = tile.ID;
            mapTextureTable_.insert(std::make_pair(tile.ID, pTileTexture));
        }
    }
}

Texture2D GameMap::getTexture(unsigned int id)
{
    auto it = mapTextureTable_.find(id);
    if (it != mapTextureTable_.end()){
        return it->second->texture;
    } else {
        throw "Texture not found";
    }
}

void GameMap::unloadTextures()
{
    for (const auto &p : mapTextureTable_)
    {
        UnloadTexture(p.second->texture);
    }
}

int GameMap::getHeight() const {
    return height_;
}

int GameMap::getWidth() const {
    return width_;
}

const Vector2 &GameMap::getSpawnPositionP1() const {
    return spawnPositionP1_;
}

const Vector2 &GameMap::getSpawnPositionP2() const {
    return spawnPostionP2_;
}

unsigned int **GameMap::getGraphicsLayer() const {
    return graphicsLayer_;
}

unsigned int **GameMap::getBackgroundLayer() const {
    return backgroundLayer_;
}

const std::map<unsigned int, TileTexture> &GameMap::getTextureTable() const {
    return mapTextureTable_;
}

int GameMap::getPixelHeight() const {
    return height_ * 32;
}

int GameMap::getPixelWidth() const {
    return width_ * 32;
}

void GameMap::loadTileEntity(
        ECS::Entity* ent,
        tmx::FloatRect AABB,
        std::vector<tmx::Property> properties,
        std::string layerName) {
    // Adjust coordinates
    float x = std::round((AABB.left * 2) / 32) * 32;
    float y = std::round((AABB.top * 2) / 32) * 32;
    float width, height;
    if (std::round(AABB.width) <= TILE_SIZE + 2) {
        width = GAME_TILE_SIZE;
    } else {
        int n = (int) std::round(AABB.width / 16);
        width = GAME_TILE_SIZE * n;
    }

    if (std::round(AABB.width) <= TILE_SIZE + 2) {
        height = GAME_TILE_SIZE;
    } else {
        int n = (int) std::round(AABB.height / 16);
        height = GAME_TILE_SIZE * n;
    }

    ECS::ComponentHandle<AABBComponent> aabb = ent->assign<AABBComponent>(Rectangle{x , y, width, height});
    ent->assign<SolidComponent>();
    ent->assign<TileComponent>();
    if (layerName == "pipes") {
        ent->assign<PipeComponent>();
        for (auto prop : properties) {
            if (prop.getName() == "has_piranha_plant" && prop.getBoolValue()) {
                ECS::World* world = ent->getWorld();
                createPiranhaPlant(world, aabb->getCenterX(), aabb->top());
            }
        }
    } else if (layerName == "bricks") ent->assign<BrickComponent>();
    else if (layerName == "ground") ent->assign<GroundComponent>();
    else if (layerName == "square_brick") ent->assign<SquareBrick>();
    else if (layerName == "question_block") {
        ent->assign<QuestionBlockComponent>();
        ent->remove<AABBComponent>();
        ent->assign<AABBComponent>(Rectangle{x, y + 0.5f, width, height});
    } else if (layerName == "enemies") {
        ent->remove<TileComponent>();
        createEnemy(ent, properties);
    } else if (layerName == "objects") {
        ent->remove<TileComponent>();
        createObject(ent, properties);
    }

    if (layerName == "bricks" || layerName == "question_block") {
        if (ent->has<BrickComponent>()) {
            ent->assign<TextureComponent>(TextureId::BRICK);
            ent->assign<BreakableComponent>();
        } else {
            ent->assign<TextureComponent>(TextureId::QUESTION_BLOCK_1);
            ent->assign<AnimationComponent>(std::vector<TextureId>{
                    TextureId::QUESTION_BLOCK_1,
                    TextureId::QUESTION_BLOCK_1,
                    TextureId::QUESTION_BLOCK_1,
                    TextureId::QUESTION_BLOCK_2,
                    TextureId::QUESTION_BLOCK_3,
                    TextureId::QUESTION_BLOCK_2,
            }, 12);
            if (!properties.empty()) {
                auto questionComponent = ent->get<QuestionBlockComponent>();
                for (const auto& property : properties) {
                    if (property.getName() == "coins" && property.getBoolValue()) {
                        questionComponent->coin = true;
                    } else if (property.getName() == "super_mario_mushroom" && property.getBoolValue()) {
                        questionComponent->superMarioMushroom = true;
                    } else if (property.getName() == "mega_mushroom" && property.getBoolValue()) {
                        questionComponent->megaMushroom = true;
                    } else if (property.getName() == "one_up_mushroom" && property.getBoolValue()) {
                        questionComponent->oneUpMushroom = true;
                    } else if (property.getName() == "flame_mushroom" && property.getBoolValue()) {
                        questionComponent->flameMushroom = true;
                    }
                }
            }
        }
        ent->assign<BounceComponent>();
    }
}

Enemy::BulletType getBulletType(const std::string stringType) {
    if (stringType == "B_BULLET_BILL") return Enemy::BulletType::B_BULLET_BILL;
    else if (stringType == "R_BULLET_BILL") return Enemy::BulletType::R_BULLET_BILL;
    else if (stringType == "BLUE_BULLET_BILL") return Enemy::BulletType::BLUE_BULLET_BILL;
    else if (stringType == "GOOMBA") return Enemy::BulletType::B_GOOMBA;
    else if (stringType == "SUPER_MUSHROOM") return Enemy::BulletType::SUPER_MUSHROOM;
    else if (stringType == "ONE_UP_MUSHROOM") return Enemy::BulletType::ONE_UP_MUSHROOM;
    else return Enemy::BulletType::NO_BULLET;
}

void GameMap::createEnemy(ECS::Entity *ent, std::vector<tmx::Property> properties) {
    bool isBig = false;
    bool left = false;
    ECS::Entity* parachute = NULL;
    Enemy::BulletType bulletType = Enemy::BulletType::NO_BULLET;

    ent->assign<GravityComponent>();
    ent->assign<KineticComponent>();
    ent->assign<FrozenComponent>();

    if (!properties.empty()) {
        for (const auto& property : properties) {
            if (property.getName() == "type") {
                setEnemyType(ent, property.getStringValue());
            } else if (property.getName() == "hasParachute" && property.getBoolValue()) {
                parachute = createParachute(ent);
            } else if (property.getName() == "isBig" && property.getBoolValue()) {
                isBig = true;
            } else if (property.getName() == "left" && property.getBoolValue()) {
                left = true;
            } else if (property.getName() == "bulletType") {
                bulletType = getBulletType(property.getStringValue());
            }
        }
    }

    if (parachute) {
        ent->get<EnemyComponent>()->hasParachute = true;
        ent->remove<WalkComponent>();
        if (ent->has<KineticComponent>()) {
            ent->get<KineticComponent>()->speedX_ = 0.0f;
            ent->get<KineticComponent>()->accX_ = 0.0f;
        }
    }

    if (isBig) {
        auto aabb = ent->get<AABBComponent>();
        ent->get<EnemyComponent>()->isBig = true;
        aabb->setWidth(aabb->collisionBox_.width * 2);
        aabb->setHeight(aabb->collisionBox_.height * 2);
        if (parachute) {
            auto parAABB = parachute->get<AABBComponent>();
            parAABB->setCenterX(aabb->getCenterX());
        }
    }

    if (ent->has<ThwompComponent>() && !ent->get<ThwompComponent>()->isVertical) {
        if (!left) {
            ECS::ComponentHandle<ThwompComponent> thwompComponent = ent->get<ThwompComponent>();
            thwompComponent->setRight();
            thwompComponent->setInitialPos(ent->get<AABBComponent>()->left());
        }
    }

    auto cannonComponent = ent->get<CannonComponent>();
    if (cannonComponent) cannonComponent->setType(bulletType);
}

void GameMap::setEnemyType(ECS::Entity *ent, std::string type) {
    if (type == "GOOMBA") {
        ent->assign<EnemyComponent>(Enemy::Type::GOOMBA);
        ent->assign<WalkComponent>();
        ent->assign<TextureComponent>(TextureId::GOOMBA_1);
        ent->assign<AnimationComponent>(std::vector<TextureId>{
           TextureId::GOOMBA_1,
           TextureId::GOOMBA_2
        }, 16);
        ent->assign<UnderTileComponent>();
    } else if (type == "GOOMBRAT") {
        ent->assign<EnemyComponent>(Enemy::Type::GOOMBRAT);
        ent->assign<WalkComponent>();
        ent->assign<TextureComponent>(TextureId::GOOMBRAT_1);
        ent->assign<AnimationComponent>(std::vector<TextureId>{
            TextureId::GOOMBRAT_1,
            TextureId::GOOMBRAT_2
        }, 16);
        ent->assign<UnderTileComponent>();
    } else if (type == "KOOPA_TROOPA") {
        auto aabb = ent->get<AABBComponent>();
        ent->assign<EnemyComponent>(Enemy::Type::KOOPA_TROOPA);
        ent->assign<WalkComponent>();
        aabb->setTop(aabb->top() - 16);
        aabb->setHeight(48);
        ent->assign<TextureComponent>(TextureId::GREEN_TURTLE_1);
        ent->assign<AnimationComponent>(std::vector<TextureId>{
                TextureId::GREEN_TURTLE_1,
                TextureId::GREEN_TURTLE_2
        }, 16);
        ent->assign<UnderTileComponent>();
    } else if (type == "RED_KOOPA_TROOPA") {
        ent->assign<EnemyComponent>(Enemy::Type::RED_KOOPA_TROOPA);
        ent->assign<UnderTileComponent>();
    } else if (type == "TARTOSSO") {
        auto aabb = ent->get<AABBComponent>();
        ent->assign<EnemyComponent>(Enemy::Type::TARTOSSO);
        ent->assign<WalkComponent>();
        aabb->setTop(aabb->top() - 16);
        aabb->setHeight(48);
        ent->assign<TartossoComponent>();
        ent->assign<TextureComponent>(TextureId::TARTOSSO_1);
        ent->assign<AnimationComponent>(std::vector<TextureId>{
                TextureId::TARTOSSO_1,
                TextureId::TARTOSSO_2
        }, 16);
        ent->assign<UnderTileComponent>();
    } else if (type == "THWOMP_V") {
        auto aabb = ent->get<AABBComponent>();
        ent->assign<EnemyComponent>(Enemy::THWOMP_V);
        ent->assign<SolidComponent>();
        ent->assign<TextureComponent>(TextureId::THWOMP_2_V);
        ent->remove<GravityComponent>();
        aabb->setHeight(GAME_TILE_SIZE * 2);
        aabb->setWidth(24 * 2);
        ent->assign<ThwompComponent>();
        ent->get<ThwompComponent>()->setInitialPos(aabb->top());
        ent->assign<UnderTileComponent>();
    } else if (type == "THWOMP_H") {
        auto aabb = ent->get<AABBComponent>();
        ent->assign<EnemyComponent>(Enemy::THWOMP_H);
        ent->assign<SolidComponent>();
        ent->assign<TextureComponent>(TextureId::THWOMP_H);
        ent->remove<GravityComponent>();
        aabb->setTop(aabb->top() - 1);
        aabb->setHeight(GAME_TILE_SIZE * 2);
        aabb->setWidth(24 * 2);
        ECS::ComponentHandle<ThwompComponent> thwompComponent = ent->assign<ThwompComponent>();
        thwompComponent->setInitialPos(aabb->right());
        thwompComponent->setHorizontal();
        ent->assign<UnderTileComponent>();
    } else if (type == "BLACK_CANNON") {
        auto aabb = ent->get<AABBComponent>();
        ent->assign<TileComponent>();
        ent->assign<TextureComponent>(TextureId::BLACK_CANNON);
        aabb->setTop(aabb->top() - GAME_TILE_SIZE);
        aabb->setHeight(GAME_TILE_SIZE * 2);
        aabb->setWidth(GAME_TILE_SIZE);
        ent->assign<CannonComponent>();
    }
}

void GameMap::createPiranhaPlant(ECS::World* world, float spawnX, float spawnY) {
    auto piranhaPlant = world->create();
    float  height = GAME_TILE_SIZE + GAME_TILE_SIZE / 2;
    piranhaPlant->assign<FrozenComponent>();
    piranhaPlant->assign<EnemyComponent>(Enemy::PIRANHA_PLANT);
    piranhaPlant->assign<AABBComponent>(Rectangle{spawnX - GAME_TILE_SIZE / 2, spawnY + height , GAME_TILE_SIZE, height});
    piranhaPlant->assign<SolidComponent>();
    piranhaPlant->assign<KineticComponent>();
    piranhaPlant->assign<TextureComponent>(TextureId::PIRANHA_PLANT_1);
    piranhaPlant->assign<VerticalGrowComponent>(384);
    piranhaPlant->assign<AnimationComponent>(std::vector<TextureId>{
        TextureId::PIRANHA_PLANT_1,
        TextureId::PIRANHA_PLANT_2
    }, 12);
    piranhaPlant->assign<UnderTileComponent>();
}

void GameMap::createObject(ECS::Entity *entity, std::vector<tmx::Property> properties) {
    if (!properties.empty()) {
        for (const auto& property : properties) {
            if (property.getName() == "type") {
                if (property.getStringValue() == "FINAL_FLAG_POLE") {
                    ECS::World* world = entity->getWorld();
                    int poleHeigth = 152 * 2;
                    entity->assign<ObjectComponent>(Object::Type::FINAL_FLAG_POLE);
                    entity->assign<TextureComponent>(TextureId::FINAL_FLAG_POLE);
                    auto aabb = entity->get<AABBComponent>();
                    aabb->setLeft(aabb->left() + 8);
                    aabb->setWidth(GAME_TILE_SIZE / 2);
                    aabb->setTop(aabb->bottom() - poleHeigth);
                    aabb->setHeight(poleHeigth);

                    auto flag = world->create();
                    flag->assign<TextureComponent>(TextureId::FINAL_FLAG);
                    flag->assign<ObjectComponent>(Object::Type::FINAL_FLAG);
                    flag->assign<AABBComponent>(Rectangle{
                        aabb->left() - GAME_TILE_SIZE + 8,
                        aabb->top() + GAME_TILE_SIZE / 2,
                       GAME_TILE_SIZE,
                       GAME_TILE_SIZE
                    });
                    flag->assign<WinnerFlagComponent>();
                } else if (property.getStringValue() == "COIN") {
                    entity->assign<ObjectComponent>(Object::Type::COIN);
                    entity->assign<TextureComponent>(TextureId::COIN_1);
                    entity->assign<AnimationComponent>(std::vector<TextureId>{
                            TextureId::COIN_1,
                            TextureId::COIN_2,
                            TextureId::COIN_3,
                            TextureId::COIN_4,
                            TextureId::COIN_5,
                            TextureId::COIN_6,
                            TextureId::COIN_7,
                            TextureId::COIN_8,
                    }, 6);
                } else if (property.getStringValue() == "COIN_10") {
                    entity->assign<ObjectComponent>(Object::Type::COIN_10);
                    entity->assign<TextureComponent>(TextureId::COIN_10_1);
                    entity->assign<AnimationComponent>(std::vector<TextureId>{
                        TextureId::COIN_10_1,
                        TextureId::COIN_10_2,
                        TextureId::COIN_10_3,
                        TextureId::COIN_10_4,
                        TextureId::COIN_10_5,
                        TextureId::COIN_10_6,
                        TextureId::COIN_10_7,
                        TextureId::COIN_10_8,
                    }, 8);
                } else if (property.getStringValue() == "COIN_30") {
                    entity->assign<ObjectComponent>(Object::Type::COIN_30);
                    entity->assign<TextureComponent>(TextureId::COIN_30_1);
                    entity->assign<AnimationComponent>(std::vector<TextureId>{
                            TextureId::COIN_30_1,
                            TextureId::COIN_30_2,
                            TextureId::COIN_30_3,
                            TextureId::COIN_30_4,
                            TextureId::COIN_30_5,
                            TextureId::COIN_30_6,
                            TextureId::COIN_30_7,
                            TextureId::COIN_30_8,
                    }, 12);
                } else if (property.getStringValue() == "COIN_50") {
                    entity->assign<ObjectComponent>(Object::Type::COIN_50);
                    entity->assign<TextureComponent>(TextureId::COIN_50_1);
                    entity->assign<AnimationComponent>(std::vector<TextureId>{
                            TextureId::COIN_50_1,
                            TextureId::COIN_50_2,
                            TextureId::COIN_50_3,
                            TextureId::COIN_50_4,
                            TextureId::COIN_50_5,
                            TextureId::COIN_50_6,
                            TextureId::COIN_50_7,
                            TextureId::COIN_50_8,
                    }, 12);
                }
            } else if (property.getName() == "left") {
                if (property.getBoolValue()) entity->assign<PoleComponent>(Object::PoleDir::LEFT);
                else entity->assign<PoleComponent>(Object::PoleDir::RIGHT);
            }
        }
    }
}

ECS::Entity * GameMap::createParachute(ECS::Entity *entity) {
    ECS::World* world = entity->getWorld();
    ECS::Entity* parachute = world->create();
    auto aabb = entity->get<AABBComponent>();
    parachute->assign<ObjectComponent>(Object::Type::PARACHUTE);
    parachute->assign<ParachuteComponent>(entity);
    parachute->assign<AABBComponent>(Rectangle{
            aabb->getCenterX() - static_cast<float>(GAME_TILE_SIZE / 2),
            aabb->top() + GAME_TILE_SIZE,
            GAME_TILE_SIZE,
            GAME_TILE_SIZE
    });
    parachute->assign<TextureComponent>(TextureId::PARACHUTE);
    return parachute;
}
