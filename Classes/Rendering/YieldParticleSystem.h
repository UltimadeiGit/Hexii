#pragma once

// TODO: Implement this properly
class YieldParticleSystem : public cocos2d::ParticleSystemQuad
{

private:
	inline bool init() {
		return initWithTotalParticles(30);
	}

public:

	CREATE_FUNC(YieldParticleSystem);

	inline void update(float dt) override {
		//size
		for (int i = 0; i < _particleCount; i++)
		{
			float t = _particleData.timeToLive[i] / this->_life;

			// Symetric curve
			float scale = cocos2d::tweenfunc::quadEaseOut(t < 0.6 ? t / 0.6f : cocos2d::tweenfunc::quadEaseOut((1 - t) / 0.4)); //cocos2d::tweenfunc::quadEaseOut(t < 0.5 ? t * 2 : (1-t) * 2);

			_particleData.size[i] = MAX(0, _startSize * scale);

			

			// Color

			// Smoothly fade up, then remain at full opacity for a bit, then fade out. This means alpha should go above 1.0f for a bit (and be clamped)
			float alphaScale = t < 0.6 ? t / 0.6f : cocos2d::tweenfunc::quadEaseOut((1 - t)/0.4);

			_particleData.colorA[i] = MIN(1.0f, _startColor.a * alphaScale);
		}

		ParticleSystemQuad::update(dt);
	}

private:
	inline float bezier(float t) {
		return t * t * (3.0f - 2.0f * t);
	}
};