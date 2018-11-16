function foo (a)
    print("foo 函数输出", a)
    return coroutine.yield(2 * a) -- 返回  2*a 的值
end
function printtable(table)
	for k,v in pairs(table) do
	 print("key=", k, "val=", v)
	end
end
co = coroutine.create(function (a , b)
    print("第一次协同程序执行输出", a, b) -- co-body 1 10
    local r,s,t = foo(a + 1)
    print(r.."===="..s.."====="..t)
    print("第二次协同程序执行输出", r)
    local r, s = coroutine.yield(a + b, a - b)  -- a，b的值为第一次调用协同程序时传入
     
    print("第三次协同程序执行输出", r, s)
    return b, "结束协同程序"                   -- b的值为第二次调用协同程序时传入
end)
        
print("main", coroutine.resume(co, 1, 10)) -- true, 4
print("--分割线----")
print("main", coroutine.resume(co, "r",1,"asd")) -- true 11 -9
print("---分割线---")
print("main", coroutine.resume(co, "x", "y")) -- true 10 end
print("---分割线---")
print("main", coroutine.resume(co, "x", "y")) -- cannot resume dead coroutine
print("---分割线---")
local table1 = {1,2,3}
table2 = table1
printtable(table2)
table1 = nil
printtable(table2)
--yield返回resume的输入(resume入参)
--resume返回yield的输出(yield入参)
--[[
function newCounter ()
  local n = 0
  local k = 0
  return function ()
    k = n
    n = n + 1
    return n
    end
end

counter = newCounter ()
print(counter())
print(counter())


local j = 22
function test ()
 j = 33
 local a =10
 local b =11
	return function ()
	  a = 1
	  b = 2
	 end
end

asd = test ()
local i = 1
repeat
  name, val = debug.getupvalue(test, i)
  if name then
    print ("index", i, name, "=", val)
    if(name == "n") then
        debug.setupvalue (counter,2,10)
    end
    i = i + 1
  end -- if
until not name

print(counter())
i = 1
repeat
	name, val = debug.getlocal(1,i)
	if name then
		print("name =", name, "value =", val)
	end
	i = i + 1
until not name

print(i,j)

xx = 10
debug.debug()
local procedure

function thread1()
	local val = 0
	while true do
		val = val + 1
		coroutine.yield(val)
	end
end

function thread2()
	while true do
		local state, val = coroutine.resume(procedure)
		print(val)
	end
end

procedure = coroutine.create(thread1)
thread2()
]]--
--[[
local obtest = {length = 10,width = 20}
function obtest:area()
	print(self.length*self.width)
end
aa = obtest
aa:area()

local tbtest = {key1 = "val1"}
tbtest.key2 = "val2"
local tbtest2 = {key3 = "val4"}
tbtest = setmetatable(tbtest, {__index=tbtest2}, {__newindex=tbtest2})
print(tbtest.key3)
print("tbtest============")
printtable(tbtest)
print("tbtest2============")
printtable(tbtest2)
tbtest.key3 = "val3"
print("tbtest============")
printtable(tbtest)
print("tbtest2============")
printtable(tbtest2)


local key = {}    -- unique key
local mt = {__index = function (t) return t[key] end}
function setDefault (t, d)
  t[key] = d
  setmetatable(t, mt)
end

local tb = {x = 1, y = 2}
setDefault(tb, 10)
print(tb.z)
print("tb===========")
printtable(tb)
]]--

-- keep a private access to original table
local _t = t

-- create proxy
t = {}

-- create metatable
local mt = {
  __index = function (t,k)
	print("*access to element " .. tostring(k))
	return _t[k]   -- access the original table
  end,

  __newindex = function (t,k,v)
	print("*update of element " .. tostring(k) ..
						 " to " .. tostring(v))
	_t[k] = v   -- update original table
  end
}
setmetatable(t, mt)

    Account = { balance=0,
                withdraw = function (self, v)
                             self.balance = self.balance - v
                           end
              }
    
    function Account:deposit (v)
      self.balance = self.balance + v
    end
	
	function Account:new (o)
      o = o or {}   -- create object if user does not provide one
      setmetatable(o, self)
      self.__index = self
      return o
    end
	
	b = Account:new()
    print(b.balance)
	b:deposit(100)
	print(b.balance)
	printtable(b)
	
	tbb = {}
	tbb = setmetatable(tbb, {__index={shit = 10}})
	print("tbb========")
	printtable(tbb)
	print("shit=========")
	print(tbb.shit)
	tbb.shit = tbb.shit + 10
	printtable(tbb)