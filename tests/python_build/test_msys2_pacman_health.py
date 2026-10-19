"""Non-destructive MSYS2 package-metadata recovery tests with fake pacman."""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]
SCRIPT=ROOT/"misc/msys2_pacman_health.sh"

class PacmanRecoveryTests(unittest.TestCase):
    def setUp(self):
        self.tmp=tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root=Path(self.tmp.name)
        self.db=self.root/"local"
        self.db.mkdir()
        self.cache=self.root/"cache"
        self.cache.mkdir()
        self.package=self.db/"tzcode-2026e-1"
        self.package.mkdir()
        self.env=os.environ.copy()
        self.env.update({
            "NOE_PACMAN_LOCAL_DB":str(self.db),
            "NOE_PACMAN_CACHE":str(self.cache),
        })
    def call(self, mode):
        return subprocess.run(["bash",str(SCRIPT),mode],env=self.env,
                              capture_output=True,text=True,check=False)
    def test_check_refuses_corrupt_db_without_changes(self):
        result=self.call("--check")
        self.assertEqual(result.returncode,3,result.stderr)
        self.assertIn("tzcode-2026e-1",result.stderr)
        self.assertFalse(list(self.root.glob("local-noe-backup-*")))
    def test_repair_missing_archive_preserves_backup(self):
        result=self.call("--repair")
        self.assertEqual(result.returncode,4,result.stderr)
        self.assertFalse((self.package/"desc").exists())
        backups=list(self.root.glob("local-noe-backup-*"))
        self.assertEqual(len(backups),1)
        self.assertTrue((backups[0]/"tzcode-2026e-1").is_dir())
    def test_valid_package_requires_no_repair(self):
        (self.package/"desc").write_text("%NAME%\ntzcode\n")
        result=self.call("--check")
        self.assertEqual(result.returncode,0,result.stderr)
        self.assertFalse(list(self.root.glob("local-noe-backup-*")))
    def test_exact_cached_archive_repair_is_backup_first(self):
        (self.cache/"tzcode-2026e-1-any.pkg.tar.zst").write_text("fixture")
        fake=self.root/"fake-pacman.sh"
        fake.write_text("#!/bin/bash\n"
                        "test -d \"$(dirname \"$NOE_PACMAN_LOCAL_DB\")/local-noe-backup-\"* || exit 9\n"
                        "test \"$1\" = '-U' || exit 10\n"
                        "printf '%%NAME%%\\ntzcode\\n' > \"$NOE_PACMAN_LOCAL_DB/tzcode-2026e-1/desc\"\n")
        fake.chmod(0o755)
        self.env["NOE_PACMAN_BIN"]=str(fake)
        result=self.call("--repair")
        self.assertEqual(result.returncode,0,result.stdout+result.stderr)
        self.assertTrue((self.package/"desc").is_file())
        self.assertEqual(len(list(self.root.glob("local-noe-backup-*"))),1)

if __name__=="__main__":
    unittest.main()
