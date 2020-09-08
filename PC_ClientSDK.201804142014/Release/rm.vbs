if MsgBox("是否清除历史数据？" ,vbOKCancel, "北斗鹰PC") = 1 then
    Const WindowsFolder = 0
    Const SystemFolder = 1
    Const TemporaryFolder = 2
    Dim fso: Set fso = CreateObject("Scripting.FileSystemObject")
    Dim tempFolder: tempFolder = fso.GetSpecialFolder(TemporaryFolder)
    tempFolder = tempFolder + "\PPCSClient"
    fso.DeleteFolder(tempFolder)
end if