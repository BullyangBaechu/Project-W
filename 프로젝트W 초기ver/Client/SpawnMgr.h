#pragma once
class SpawnMgr
{
	SINGLE(SpawnMgr);

private:
    float m_SpawnDelay;                     // 초기 스폰 간격 시간
    float m_MinSpawnDelay;                  // 최단 스폰 간격
    float m_DifficultyTime;                 // 난이도 누적 시간
    float m_DelayDescreaseTime;             // 몇 초마다 시간을 줄인걸지
    float m_AccTime;                        // 누적시간

    bool m_FirstSpawn = true;               // 첫 생성 오브젝트 고정 용

    
    STAGEMODE m_StageMode;

private:
    void SpawnRandomObject();


public:
    void Init();
    void Tick();
    //virtual void FinalTick() override;

    void SetSpawnDelay(float delay) { m_SpawnDelay = delay; }
    float GetSpawnDelay() const { return m_SpawnDelay; }
    GET_SET(STAGEMODE, StageMode);

 
};

