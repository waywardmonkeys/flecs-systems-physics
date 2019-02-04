/*
                                   )
                                  (.)
                                  .|.
                                  | |
                              _.--| |--._
                           .-';  ;`-'& ; `&.
                          \   &  ;    &   &_/
                           |"""---...---"""|
                           \ | | | | | | | /
                            `---.|.|.|.---'

 * This file is generated by bake.lang.c for your convenience. Headers of
 * dependencies will automatically show up in this file. Include bake_config.h
 * in your main project file. Do not edit! */

#ifndef REFLECS_SYSTEMS_PHYSICS_BAKE_CONFIG_H
#define REFLECS_SYSTEMS_PHYSICS_BAKE_CONFIG_H

/* Headers of public dependencies */
#include <reflecs>
#include <reflecs.math>
#include <reflecs.components.transform>
#include <reflecs.components.geometry>
#include <reflecs.components.physics>
#include <reflecs.systems.transform>

/* Headers of private dependencies */
#ifdef REFLECS_SYSTEMS_PHYSICS_IMPL
/* No dependencies */
#endif

/* Convenience macro for exporting symbols */
#if REFLECS_SYSTEMS_PHYSICS_IMPL && defined _MSC_VER
#define REFLECS_SYSTEMS_PHYSICS_EXPORT __declspec(dllexport)
#elif REFLECS_SYSTEMS_PHYSICS_IMPL
#define REFLECS_SYSTEMS_PHYSICS_EXPORT __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define REFLECS_SYSTEMS_PHYSICS_EXPORT __declspec(dllimport)
#else
#define REFLECS_SYSTEMS_PHYSICS_EXPORT
#endif

#endif

