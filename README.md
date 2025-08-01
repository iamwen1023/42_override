# 42_override

```
qemu-system-x86_64 -cdrom ~/Downloads/OverRide.iso -m 4G -netdev user,id=net0,hostfwd=tcp::8022-:4242 -device virtio-net-pci,netdev=net0
```

```
ssh level00@127.0.0.1 -p 8022 

