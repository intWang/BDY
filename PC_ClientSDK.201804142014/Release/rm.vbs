if MsgBox("是否清除历史数据？" ,vbOKCancel, "北斗鹰PC") = 1 then
    Set obj = createobject("wscript.shell")
    obj.run ".\rm.bat",1
end if