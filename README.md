# Usage
```
hugetest [SIZE] [HUGE]
 
  MEGABYTES = Amount of memory to use in megabytes (MB)

  HUGE = 1 to request hugepages (MADV_HUGEPAGE)
       = 0 to disable hugepages (MADV_NOHUGEPAGE)
```

# Example

```$ ./hugetest 1024 0
allocating memory...
1073741824 allocated at: 0x7f3080400000
disabling hugepages...
touching pages...
running test...
test complete in: 12.917s```

```$ ./hugetest 1024 1
allocating memory...
1073741824 allocated at: 0x7efbf4800000
requesting hugepages...
touching pages...
running test...
test complete in: 9.954s```
