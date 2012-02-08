-- bc package was derived from
-- http://penlight.luaforge.net/packages/lbc.html
--
require"bc"

bc.digits (250000)

fact = bc.number(1)
N = 50000

for i=2,N do
   fact = bc.mul(fact, i)
end

--print(fact)
print(N,"! calculation is finished!")

