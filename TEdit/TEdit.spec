# -*- mode: python -*-

block_cipher = None


a = Analysis(['TEdit.py'],
             pathex=['/Users/Taris/workshop/TEdit'],
             hiddenimports=[],
             hookspath=None,
             runtime_hooks=None,
             excludes=None,
             cipher=block_cipher)
pyz = PYZ(a.pure,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          name='TEdit',
          debug=False,
          strip=None,
          upx=True,
          console=False )
app = BUNDLE(exe,
             name='TEdit.app',
             icon=None,
             bundle_identifier=None)
