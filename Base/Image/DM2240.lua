function SaveToLuaFile(outputString, overwrite)
   print("SaveToLuaFile...")
   local f;						-- The file
   if overwrite == 1 then		-- Wipe the contents with new data
      f = assert(io.open("Image/DM2240_HighScore.lua", "w"))
   elseif overwrite == 0 then	-- Append with new data
      f = assert(io.open("Image/DM2240_HighScore.lua", "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
end

function CalculateDistanceSquare(x1,y1,z1,x2,y2,z2)
	local distanceSquare = (x2-x1)*(x2-x1) + (y2 -y1) * (y2 -y1) + (z2 -z1) * (z2 -z1)
	print(distanceSquare)
	return distanceSquare
end

function GetMinMax(...)
	local minValue = -1
	local maxValue = -1
	local aveValue = 0
	local numValues = 0
	
	print(minValue,maxValue,aveValue,numValues)
	
	for i,v in ipairs(arg) do
		if minValue == -1 then
			minValue = v
		elseif v < minValue then
			minValue = v
		end
	
		if maxValue == -1 then
			maxValue = v
		elseif v>maxValue then
			maxValue = v
		end
		
		aveValue = (aveValue*numValues + v)/(numValues + 1)
		numValues = numValues + 1
	
	print(minValue,maxValue,aveValue,numValues)
	
	end
	--aveValue = aveValue/numValues
	--print(minValue,maxValue,aveValue,numValues)
	return minValue,maxValue,aveValue,numValues
	
end

title = "DM2240 - Week 14 Scripting"
width = 800
height = 600
PlayerPos_x = 100
PlayerPos_z = 200
keyFORWARD = "Z";

--Keyboard Inputs
moveForward = "W"
moveBackward = "S"
moveLeft = "A"
moveRight = "D"

--CPlayerInfo Init
CPlayerInfoStartPos = {x =0,y =0,z =10}
CPlayerInfoTargetPos = {x =0,y =0,z =0}
CPlayerInfoJumpAcc = -10
CPlayerInfoFallAcc = -10
CPlayerInfoLife = 100

--WeaponInfo Init
--Pistol
CPistolMagRounds = 8
CPistolMaxMagRounds = 8
CPistolTotalRounds = 40
CPistolMaxTotalRounds = 40
CPistolShotCooldown = 0.333

--Waypoints
Waypoint_A_1 = {x = 10.0, y=0.0, z=50.0}
Waypoint_A_2 = {x = 10.0, y=0.0, z=-50.0}
Waypoint_A_3 = {x = 10.0, y=0.0, z=0.0}