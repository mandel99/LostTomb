param([Parameter(Mandatory=$true)][int]$GhidraPid,
 [ValidateSet('placement','warnings','sites')][string]$Cohort='placement')
$ErrorActionPreference='Stop'
$clean=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$root=Split-Path (Split-Path $clean -Parent) -Parent
$out=Join-Path $env:TEMP 'losttomb-ghidra-native-comparison'
$jdk=Join-Path $env:USERPROFILE '.codex/tools/jdk-21.0.12.101-hotspot/bin'
$ghidra=Join-Path $root 'ghidra_12.1.4_PUBLIC'
$lib=Join-Path $clean 'build/bootstrap-vs2022-win32/Release/pharaoh_game.lib'
if(-not(Test-Path -LiteralPath $lib)){throw 'Build the Release pharaoh_clean_bootstrap target first'}
$null=Get-Process -Id $GhidraPid -ErrorAction Stop
New-Item -ItemType Directory -Path $out -Force|Out-Null
foreach($name in @('LostTombNativeComparisonAgent.java','LostTombAttach.java','fixture-placement.c','compare-placement.py','run-placement-cohort.py','run-placement-extra.py','placement-spec.json','fixture-warnings.c','compare-warnings.py','run-warnings-cohort.py','warnings-spec.json','fixture-sites.c','run-site-cohort.py','run-root-support-cohort.py')){Copy-Item -LiteralPath (Join-Path $PSScriptRoot $name) -Destination (Join-Path $out $name) -Force}
$live=$false
if(Test-Path -LiteralPath "$out/ready.json"){
 try{$v=Get-Content -Raw "$out/ready.json"|ConvertFrom-Json;$r=Invoke-RestMethod -Uri ("http://127.0.0.1:"+$v.port+'/info') -Headers @{'X-LostTomb-Token'=$v.token} -TimeoutSec 5;$live=$r.ok -and $r.sha256 -eq 'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'}catch{$live=$false}
}
if(-not $live){
$cp=(@(Get-ChildItem -LiteralPath $ghidra -Recurse -File -Filter '*.jar'|ForEach-Object {$_.FullName}) -join ';')
$argsList=@('-encoding','UTF-8','-classpath',$cp,'-d',$out,(Join-Path $out 'LostTombNativeComparisonAgent.java'),(Join-Path $out 'LostTombAttach.java'))
$argsText=($argsList|ForEach-Object {'"'+$_.Replace('\','/')+'"'}) -join "`n"
[IO.File]::WriteAllText((Join-Path $out 'compile.args'),$argsText)
& (Join-Path $jdk 'javac.exe') "@$out/compile.args"
if($LASTEXITCODE){throw 'Diagnostic agent compilation failed'}
[IO.File]::WriteAllText((Join-Path $out 'MANIFEST.MF'),"Manifest-Version: 1.0`nAgent-Class: LostTombNativeComparisonAgent`nCan-Redefine-Classes: false`nCan-Retransform-Classes: false`n`n")
& (Join-Path $jdk 'jar.exe') --create --file "$out/agent.jar" --manifest "$out/MANIFEST.MF" -C $out LostTombNativeComparisonAgent.class -C $out LostTombAttach.class
if($LASTEXITCODE){throw 'Diagnostic JAR creation failed'}
& (Join-Path $jdk 'java.exe') --add-modules jdk.attach -cp "$out/agent.jar" LostTombAttach $GhidraPid "$out/agent.jar" $out;if($LASTEXITCODE){throw 'Attach to existing Ghidra JVM failed'}
}
$vc='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Auxiliary/Build/vcvars32.bat'
$runtime='/MD';$linkOptions='';$runtimeLibs=''
if($Cohort -ne 'placement'){$runtime='/MT';$linkOptions='/NODEFAULTLIB:MSVCRT ';$runtimeLibs='libcmt.lib libvcruntime.lib libucrt.lib kernel32.lib ucrt.lib '}
$lines=@('@echo off',('call "'+$vc+'" >nul'),'if errorlevel 1 exit /b %errorlevel%',('cl.exe /nologo /std:c11 /W4 /WX /O2 /Oi /Gy /GS- '+$runtime+' /I"'+$clean+'/game/include" /Fo"'+$out+'/fixture-'+$Cohort+'.obj" /c "'+$out+'/fixture-'+$Cohort+'.c"'),'if errorlevel 1 exit /b %errorlevel%',('link.exe /nologo /DLL /NOENTRY /MACHINE:X86 /BASE:0x10000000 /FIXED /DYNAMICBASE:NO /OPT:REF '+$linkOptions+'/OUT:"'+$out+'/'+$Cohort+'-fixture.dll" /MAP:"'+$out+'/'+$Cohort+'-fixture.map" "'+$out+'/fixture-'+$Cohort+'.obj" "'+$lib+'" '+$runtimeLibs+'user32.lib gdi32.lib winmm.lib ole32.lib shell32.lib comdlg32.lib advapi32.lib msimg32.lib dxguid.lib ddraw.lib'),'exit /b %errorlevel%')
[IO.File]::WriteAllText((Join-Path $out ('build-'+$Cohort+'.cmd')),($lines -join "`r`n")+"`r`n")
& cmd.exe /c "$out/build-$Cohort.cmd"
if($LASTEXITCODE){throw 'Fixture DLL link failed'}
if($Cohort -ne 'placement'){Write-Output ('Prepared '+$Cohort+' cohort with actual static CRT memory routines');return}
$map=Get-Content -Raw (Join-Path $out 'placement-fixture.map')
$targets=@{}
foreach($name in @('pharaoh_monument_no_progress','pharaoh_monument_tile_is_free','pharaoh_abu_simbel_tile_is_free','pharaoh_royal_tomb_terrain_allowed','pharaoh_lighthouse_tile_is_free')){
 $m=[regex]::Match($map,'\s_'+$name+'\s+([0-9A-Fa-f]{8})\s')
 if(-not $m.Success){throw ('Actual C trace symbol absent: '+$name)}
 $targets[$name]=[Convert]::ToInt64($m.Groups[1].Value,16)
}
[IO.File]::WriteAllText((Join-Path $out 'placement-trace-addresses.json'),($targets|ConvertTo-Json -Compress))
Write-Output 'Prepared the single cohort fixture; verify ready.json before running comparisons'
