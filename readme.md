<h1 align="center">
    <img src="public/img/logo.png" width="256"/><br/>
    Apocalypse Server
</h1>

<p align="center">
    A server-side hack for GoldSrc CS/CZ<br/>
    (C) 2014 - 2025 by Daniel Brendel
</p>

<p align="center">
    <img src="https://img.shields.io/badge/lang-cpp-green" alt="lang-cpp"/>
    <img src="https://img.shields.io/badge/engine-goldsrc-orange" alt="engine-goldsrc"/>
    <img src="https://img.shields.io/badge/mods-cs/cz-violet" alt="mods-cs/cz"/>
	<img src="https://img.shields.io/badge/license-MIT-blue" alt="license-mit"/>
</p>

<p align="center">
    <img src=""/>
</p>

## Author

__Author__: Daniel Brendel\
__Contact__: dbrendel1988(at)gmail(dot)com\
__GitHub__: https://github.com/danielbrendel

## Description
This is a server-side hack for GoldSrc CS/CZ, based on the <a href="https://github.com/danielbrendel/dnySSHBase">SSHBase</a>. It works both with Counter-Strike 1.6 and Condition Zero, including ZBots. It can be built for Windows and Linux. 

## Disclaimer

This project is meant for educational purposes only. End-user support is not provided.

## Features:
- Bunnyhop
- Invincibility
- Noclip
- Health care
- Gravity
- Player glow
- Transparency
- Item spawner
- Zone spawner (buy, bomb, hostage)
- Hostage spawner
- Traceaim
- Rocket launcher
- Energy Ball
- Mortar
- Save/Restore position
- Action menu
	- Slap player
	- Blind player
	- Play step near player
	- Slay player
	- Kick player
	- Spawn hostage
	- Spawn rescue zone
	- Spawn bomb zone
	- Spawn buy zone

## Installation

### Windows
- Copy the `ApocServ.dll` and `ApocServ.cfg` to `cstrike\dlls` or `czero\dlls`
- Edit the `ApocServ.cfg` to fit your specific needs
- Additionally manage your SteamID specific configs (see `localid.cfg` for details)
- Set the game dll via the `cstrike\liblist.gam` or `czero\liblist.gam`
	```
	gamedll "dlls\ApocServ.dll"
	```
- Do not remove the original game dll
- Now (re)start your server

### Linux
- Copy the `ApocServ.so` and `ApocServ.cfg` to `cstrike/dlls` or `czero/dlls`
- Edit the `ApocServ.cfg` to fit your specific needs
- Additionally manage your SteamID specific configs (see `localid.cfg` for details)
- Set the game library via the `cstrike\liblist.gam` or `czero\liblist.gam`
	```
	gamedll_linux "dlls/ApocServ.so"
	```
- Do not remove the original game library
- Now (re)start your server

## Usage

- Set the server hack authentication password
	```
	// Direct server access
	ssh_password "your-auth-password"
	// Remote access
	rcon ssh_password "your-auth-password"
	```
- Players can now authenticate via the password
	```
	ssh_auth "your-auth-password"
	```
- Now you can start using the hack
- Use `ssh_help` for a list of available features
