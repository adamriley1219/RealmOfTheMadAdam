##Change Checklist
- All changes made to code must follow these rules beforehand.

Get to bottlenecks by finding MS going down.
Pinpoint where the problem resides by using MS.
Move on to refactoring to optimize the code.
Retest to see if the MS has any improvements.

##Finding a good test case

- 606 entities stat at about 50 FPS
	- Goal is 60+



##Pain Point Analysis

3 enemies
50/50 - Debug 30 fps, Release 300-500 fps 


8106 entities - 0 FPS
1040 entities - ~20 FPS
535 entities - ~60-70 FPS
600 entities - 50-60 FPS sitting ~54
600 entities all firing at the player with a 8 unit (one tile) radius creating ~850-900 entities
	- ~30 FPS
-

##Update of Rendering pipeline from engine intergeneration

about 300 entities gives around 200 FPS before the update.

After update - No noticeable difference. 
- No action taken.

##Profiling

600 entities
	- LogProfileScope:Update took 0.012970 (12970300 hpc)
	- LogProfileScope:Render took 0.000706 (706000 hpc)
		- Update + Render = 0.013676
		- Update %94.8
		- Render %05.2
	- LogProfileScope:Update took 0.013244 (13244300 hpc)
	- LogProfileScope:Render took 0.001077 (1077100 hpc)
		- Update + Render = 0.014321
		- Update %92.4
		- Render %07.6
	- LogProfileScope:Update took 0.014332 (14331500 hpc)
	- LogProfileScope:Render took 0.000746 (746300 hpc)
	- LogProfileScope:Update took 0.013242 (13241500 hpc)
	- LogProfileScope:Render took 0.000723 (722800 hpc)
	- LogProfileScope:Update took 0.014096 (14096100 hpc)
	- LogProfileScope:Render took 0.000687 (686900 hpc)
-

#Update

-
LogProfileScope:AISystem took 0.007497 (7497100 hpc)
LogProfileScope:AISystem took 0.006670 (6669500 hpc)
LogProfileScope:AISystem took 0.006681 (6680500 hpc)
LogProfileScope:AISystem took 0.006502 (6501900 hpc)
LogProfileScope:AISystem took 0.011520 (11520100 hpc)
-
LogProfileScope:CombatSystem took 0.000006 (5700 hpc)
LogProfileScope:CombatSystem took 0.000006 (5700 hpc)
LogProfileScope:CombatSystem took 0.000011 (11000 hpc)
LogProfileScope:CombatSystem took 0.000006 (5900 hpc)
LogProfileScope:CombatSystem took 0.000006 (5800 hpc)
-
LogProfileScope:GameInputSystem took 0.000001 (600 hpc)
LogProfileScope:GameInputSystem took 0.000001 (600 hpc)
LogProfileScope:GameInputSystem took 0.000001 (600 hpc)
LogProfileScope:GameInputSystem took 0.000001 (700 hpc)
LogProfileScope:GameInputSystem took 0.000001 (1400 hpc)
-
LogProfileScope:GamePhysicsSystem took 0.009584 (9583600 hpc)
LogProfileScope:GamePhysicsSystem took 0.008604 (8603900 hpc)
LogProfileScope:GamePhysicsSystem took 0.007648 (7647500 hpc)
LogProfileScope:GamePhysicsSystem took 0.009854 (9853700 hpc)
LogProfileScope:GamePhysicsSystem took 0.010275 (10274800 hpc)
-
LogProfileScope:MovementSystem took 0.000015 (15100 hpc)
LogProfileScope:MovementSystem took 0.000015 (14800 hpc)
LogProfileScope:MovementSystem took 0.000066 (65600 hpc)
LogProfileScope:MovementSystem took 0.000021 (20800 hpc)
LogProfileScope:MovementSystem took 0.000016 (15600 hpc)
-
LogProfileScope:QuestSystem took 0.000002 (2400 hpc)
LogProfileScope:QuestSystem took 0.000003 (2500 hpc)
LogProfileScope:QuestSystem took 0.000002 (2400 hpc)
LogProfileScope:QuestSystem took 0.000024 (24000 hpc)
LogProfileScope:QuestSystem took 0.000003 (2500 hpc)
-
LogProfileScope:RenderSystem:Update took 0.000030 (29500 hpc)
LogProfileScope:RenderSystem:Update took 0.000033 (32500 hpc)
LogProfileScope:RenderSystem:Update took 0.000040 (40200 hpc)
LogProfileScope:RenderSystem:Update took 0.000046 (45900 hpc)
LogProfileScope:RenderSystem:Update took 0.000029 (29300 hpc)
-
LogProfileScope:TriggerSystem took 0.000073 (72600 hpc)
LogProfileScope:TriggerSystem took 0.000069 (68600 hpc)
LogProfileScope:TriggerSystem took 0.000117 (116500 hpc)
LogProfileScope:TriggerSystem took 0.000226 (226200 hpc)
LogProfileScope:TriggerSystem took 0.000296 (296400 hpc)
-

#Render

-
LogProfileScope:UISystem::Render took 0.000033 (32600 hpc)
LogProfileScope:UISystem::Render took 0.000033 (32700 hpc)
LogProfileScope:UISystem::Render took 0.000032 (31600 hpc)
LogProfileScope:UISystem::Render took 0.000031 (31400 hpc)
LogProfileScope:UISystem::Render took 0.000033 (33100 hpc)
-
LogProfileScope:RenderSystem:Render took 0.000411 (411300 hpc)
LogProfileScope:RenderSystem:Render took 0.000356 (356100 hpc)
LogProfileScope:RenderSystem:Render took 0.000434 (434400 hpc)
LogProfileScope:RenderSystem:Render took 0.000371 (370700 hpc)
LogProfileScope:RenderSystem:Render took 0.000372 (372200 hpc)
-

4 entities
LogProfileScope:Update took 0.000109 (109400 hpc)
LogProfileScope:Render took 0.000139 (138600 hpc)
LogProfileScope:Update took 0.000112 (112100 hpc)
LogProfileScope:Render took 0.000137 (137000 hpc)
LogProfileScope:Update took 0.000134 (133800 hpc)
LogProfileScope:Render took 0.000269 (269400 hpc)
LogProfileScope:Update took 0.000224 (224000 hpc)
LogProfileScope:Render took 0.000298 (298000 hpc)
LogProfileScope:Update took 0.000142 (142200 hpc)
LogProfileScope:Render took 0.000215 (215200 hpc)
-

#Update

-
LogProfileScope:AISystem took 0.000006 (5900 hpc)
LogProfileScope:AISystem took 0.000006 (6300 hpc)
LogProfileScope:AISystem took 0.000007 (7000 hpc)
LogProfileScope:AISystem took 0.000010 (9800 hpc)
LogProfileScope:AISystem took 0.000011 (10600 hpc)
-
LogProfileScope:CombatSystem took 0.000006 (6100 hpc)
LogProfileScope:CombatSystem took 0.000008 (7800 hpc)
LogProfileScope:CombatSystem took 0.000011 (11100 hpc)
LogProfileScope:CombatSystem took 0.000011 (10700 hpc)
LogProfileScope:CombatSystem took 0.000006 (5800 hpc)
-
LogProfileScope:GameInputSystem took 0.000001 (1300 hpc)
LogProfileScope:GameInputSystem took 0.000001 (500 hpc)
LogProfileScope:GameInputSystem took 0.000001 (500 hpc)
LogProfileScope:GameInputSystem took 0.000001 (500 hpc)
LogProfileScope:GameInputSystem took 0.000001 (600 hpc)
-
LogProfileScope:GamePhysicsSystem took 0.000037 (36700 hpc)
LogProfileScope:GamePhysicsSystem took 0.000059 (59000 hpc)
LogProfileScope:GamePhysicsSystem took 0.000034 (34400 hpc)
LogProfileScope:GamePhysicsSystem took 0.000024 (24200 hpc)
LogProfileScope:GamePhysicsSystem took 0.000023 (22800 hpc)
-
LogProfileScope:MovementSystem took 0.000008 (7600 hpc)
LogProfileScope:MovementSystem took 0.000016 (15500 hpc)
LogProfileScope:MovementSystem took 0.000009 (8800 hpc)
LogProfileScope:MovementSystem took 0.000008 (7500 hpc)
LogProfileScope:MovementSystem took 0.000008 (7600 hpc)
-
LogProfileScope:QuestSystem took 0.000001 (1100 hpc)
LogProfileScope:QuestSystem took 0.000001 (1300 hpc)
LogProfileScope:QuestSystem took 0.000002 (1700 hpc)
LogProfileScope:QuestSystem took 0.000001 (700 hpc)
LogProfileScope:QuestSystem took 0.000001 (1100 hpc)
-
LogProfileScope:RenderSystem:Update took 0.000039 (39000 hpc)
LogProfileScope:RenderSystem:Update took 0.000040 (39500 hpc)
LogProfileScope:RenderSystem:Update took 0.000036 (35700 hpc)
LogProfileScope:RenderSystem:Update took 0.000026 (26400 hpc)
LogProfileScope:RenderSystem:Update took 0.000018 (18400 hpc)
-
LogProfileScope:TriggerSystem took 0.000001 (500 hpc)
LogProfileScope:TriggerSystem took 0.000000 (300 hpc)
LogProfileScope:TriggerSystem took 0.000000 (400 hpc)
LogProfileScope:TriggerSystem took 0.000000 (400 hpc)
LogProfileScope:TriggerSystem took 0.000000 (400 hpc)
-

#Render

LogProfileScope:UISystem::Render took 0.000029 (28900 hpc)
LogProfileScope:UISystem::Render took 0.000045 (44500 hpc)
LogProfileScope:UISystem::Render took 0.000075 (75300 hpc)
LogProfileScope:UISystem::Render took 0.000031 (30800 hpc)
LogProfileScope:UISystem::Render took 0.000031 (31100 hpc)
-
LogProfileScope:RenderSystem:Render took 0.000065 (64800 hpc)
LogProfileScope:RenderSystem:Render took 0.000057 (56600 hpc)
LogProfileScope:RenderSystem:Render took 0.000065 (64600 hpc)
LogProfileScope:RenderSystem:Render took 0.000117 (116800 hpc)
LogProfileScope:RenderSystem:Render took 0.000107 (106700 hpc)
-