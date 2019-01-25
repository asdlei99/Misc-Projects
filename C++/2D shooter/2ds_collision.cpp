#include "2ds_collision.h"

Collision::Collision(bool renderBounds) {
	this->walls = gMap->currentLevelWalls;
	this->renderBounds = renderBounds;
}

void Collision::AddBoundingBox(Vector2 pos, int height, int width, bool isColliding) {
	SDL_Rect box;
	box.x = pos.x;
	box.y = pos.y;
	box.w = width;
	box.h = height;
	boxes.push_back(std::make_pair(box, isColliding));
}

bool Collision::CheckPlayerHit(Projectile enemyBullet) {
	double playerX = gPlayer->pos.x;
	double playerY = gPlayer->pos.y;
	double bulletX = enemyBullet.x;
	double bulletY = enemyBullet.y;
	int enemyBulletSize = enemyBullet.bulletSize;
	int playerSize = gPlayer->playerSize;

	double d = sqrt(((playerX - bulletX) * (playerX - bulletX)) + ((playerY - bulletY) * (playerY - bulletY)));

	if (d > 48) {
		return false;
	}

	if (((bulletX - playerX) * (bulletX - playerX) + (bulletY - playerY) * (bulletY - playerY)) < ((enemyBulletSize + 20) * (enemyBulletSize + 20))) {

		if (renderBounds) {
			AddBoundingBox(Vector2(playerX - playerSize / 2, playerY - playerSize / 2), playerSize, playerSize, true);
			AddBoundingBox(Vector2(bulletX - enemyBulletSize / 2, bulletY - enemyBulletSize / 2), enemyBulletSize, enemyBulletSize, true);
		}

		return true;
	} else {

		if (renderBounds) {
			AddBoundingBox(Vector2(playerX - playerSize / 2, playerY - playerSize / 2), playerSize, playerSize, false);
			AddBoundingBox(Vector2(bulletX - enemyBulletSize / 2, bulletY - enemyBulletSize / 2), enemyBulletSize, enemyBulletSize, false);
		}

	}
	return false;
}

bool Collision::CheckEnemyHit(Enemy enemy) {

	for (int j = 0; j < gPlayer->projectiles.size(); j++) {
		double enemyX = enemy.pos.x;
		double enemyY = enemy.pos.y;
		double bulletX = gPlayer->projectiles[j].x;
		double bulletY = gPlayer->projectiles[j].y;
		int playerSize = gPlayer->playerSize;
		int bulletSize = gPlayer->projectiles[j].bulletSize;

		double d = sqrt(((enemyX - bulletX) * (enemyX - bulletX)) + ((enemyY - bulletY) * (enemyY - bulletY)));

		if (d > 48) {
			continue;
		}

		if (((bulletX - enemyX) * (bulletX - enemyX) + (bulletY - enemyY) * (bulletY - enemyY)) < ((bulletSize + 20) * (bulletSize + 20))) {
			//Remove the bullet on collision
			gPlayer->projectiles.erase(gPlayer->projectiles.begin() + j);

			if (renderBounds) {
				AddBoundingBox(Vector2(enemyX - playerSize / 2, enemyY - playerSize / 2), playerSize, playerSize, true);
				AddBoundingBox(Vector2(bulletX - bulletSize / 2, bulletY - bulletSize / 2), bulletSize, bulletSize, true);
			}

			return true;
		} else {

			if (renderBounds) {
				AddBoundingBox(Vector2(enemyX - playerSize / 2, enemyY - playerSize / 2), playerSize, playerSize, false);
				AddBoundingBox(Vector2(bulletX - bulletSize / 2, bulletY - bulletSize / 2), bulletSize, bulletSize, false);
			}

		}

	}
	return false;
}

void Collision::UpdateWalls() {
	this->walls = gMap->currentLevelWalls;
}

bool Collision::CollisionWithWalls(Vector2 movingToPos) { //Check collison with walls

	for (int i = 0; i < this->walls.size(); i++) {
		Vector2 wallCenter = walls[i].GetCenter();
		int wallX = this->walls[i].position.x;
		int wallY = this->walls[i].position.y;
		int wallWidth = walls[i].tileWidth;
		int wallHeight = walls[i].tileHeight;
		int playerSize;

		//Get distance from wall
		//double d = sqrt(((wallX - movingToPos.x) * (wallX - movingToPos.x)) + ((wallY - movingToPos.y) * (wallY - movingToPos.y)));
		double d = sqrt(((wallCenter.x - movingToPos.x) * (wallCenter.x - movingToPos.x)) + ((wallCenter.y - movingToPos.y) * (wallCenter.y - movingToPos.y)));

		//Only do collision detection if entity is close enough
		if (d < 48) {

			if (wallX <= movingToPos.x &&
				wallX + wallWidth >= movingToPos.x &&
				wallY <= movingToPos.y &&
				wallY + wallHeight >= movingToPos.y) {

				if (renderBounds) {
					playerSize = gPlayer->playerSize;
					AddBoundingBox(Vector2(movingToPos.x - playerSize / 2, movingToPos.y - playerSize / 2), playerSize, playerSize, true);
					AddBoundingBox(Vector2(wallX, wallY), wallHeight, wallWidth, true);
				}

				return true;
			}

			if (renderBounds) {
				playerSize = gPlayer->playerSize;
				AddBoundingBox(Vector2(movingToPos.x - playerSize / 2, movingToPos.y - playerSize / 2), playerSize, playerSize, false);
				AddBoundingBox(Vector2(wallX, wallY), wallHeight, wallWidth, false);
			}

		}

	}
	return false;
}

bool Collision::RayCastCollision(Vector2 point) {

	for (int i = 0; i < this->walls.size(); i++) {
		Vector2 wallCenter = walls[i].GetCenter();
		int wallWidth = walls[i].tileWidth;
		int wallHeight = walls[i].tileHeight;
		int wallX = walls[i].position.x;
		int wallY = walls[i].position.y;
		int playerSize;

		//Get distance from wall
		double d = sqrt(((wallCenter.x - point.x) * (wallCenter.x - point.x)) + ((wallCenter.y - point.y) * (wallCenter.y - point.y)));

		//Only do collision detection if entity is close enough
		if (d < 48) {

			if (wallX <= point.x &&
				wallX + wallWidth >= point.x &&
				wallY <= point.y &&
				wallY + wallHeight >= point.y) {

				if (renderBounds) {
					playerSize = gPlayer->playerSize;
					AddBoundingBox(Vector2(point.x - playerSize / 2, point.y - playerSize / 2), playerSize, playerSize, true);
					AddBoundingBox(Vector2(wallX, wallY), wallHeight, wallWidth, true);
				}

				return true;
			}

			if (renderBounds) {
				playerSize = gPlayer->playerSize;
				AddBoundingBox(Vector2(point.x - playerSize / 2, point.y - playerSize / 2), playerSize, playerSize, false);
				AddBoundingBox(Vector2(wallX, wallY), wallHeight, wallWidth, false);
			}

		}

	}
	return false;
}

Collision::~Collision() {

}