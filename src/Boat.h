#pragma once

class Boat {

	public:
		static void InitModel();
		static void DestroyModel();

		Boat();
		void update(float dt);
		void draw();

	private:
		static glModelData model;

}