#pragma once
#include "context.h"

/** 
*加载系统配置项
*@param config 指向SystemConfig配置项结构体的指针。用于存储加载的配置数据
*@return 成功返回true,失败返回false
**/
bool loadSystemConfig(SystemConfig *config);

/**
 * 保存系统配置项
 *@param config 指向SystemConfig配置项结构体的常量指针。用于存储加载的配置数据
 *@return 成功返回True,失败返回False
 **/
bool saveSystemConfig(const SystemConfig *config);