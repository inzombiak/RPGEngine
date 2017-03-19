MovementScript = 
{
	_steps = 0,
	_speed = 100;
	_maxSteps = 100,
	_direction = 0
}

function MovementScript:randomDirection(prevDir)
	local result = 0
	repeat
		result = math.random(0, 330)
	until math.abs(result - prevDir) > 29
	return result
end

function MovementScript:Update(time)
	if(self._steps == self._maxSteps) then
		self._direction = self:randomDirection(self._direction)
		self._steps = 0;
	end
	local currPos = self:GetPos()
	currPos.x = currPos.x + math.cos(self._direction)*self._speed*time
	currPos.y = currPos.y + math.sin(self._direction)*self._speed*time
	self:SetPos(currPos)
	self._steps = self._steps+1
end