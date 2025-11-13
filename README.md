Custom 2D Game Engine + Sample Game (WinAPI / C++)

이 프로젝트는 WinAPI 기반으로 직접 제작한 2D 게임 엔진이며,
Unity/Unreal의 구조를 참고해 Scene, Object, Component, Manager 시스템을
처음부터 끝까지 직접 구현한 개인 연구 프로젝트입니다.

🚀 Features

✔ Custom Engine Core

Game Loop (Engine → Core → Scene)

DeltaTime 계산 / FPS 관리

Input Manager 구현 (KeyMgr)

✔ Manager 기반 구조

AssetMgr (Texture/Flipbook 로딩)

RenderMgr (더블 버퍼링, 카메라 렌더링)

SpawnMgr (오브젝트 생성)

CollisionMgr (AABB 충돌)

UIMgr, EventMgr, CameraMgr 등 20종 매니저

✔ Object / Component 시스템

Actor / Object / Component 구조

Transform / Renderer / Collider / FlipbookPlayer

Unity-style Update 흐름

✔ Animation (Flipbook)

스프라이트 시트 기반 애니메이션

가변 프레임 속도

State 기반 전환

✔ Scene System

Load / Awake / Start / Update / Render 구조

TitleScene / StageScene 등

✔ UI 시스템 (Custom Widget Framework)

PanelUI, ButtonUI, TextUI, ProgressBarUI

✔ Debug Tools

충돌 디버거

🧰 Tech Stack

C++17

WinAPI / GDI

Object/Component Architecture

Custom Rendering Pipeline

STL

📜 License

개인 학습 및 포트폴리오 목적으로 제작되었습니다.
