# Homework01: Guide to Setup My Linux Machine in CloudVeneto for Compiling and Running C Codes

# CloudVeneto Setup

Once successfully accessed a project in [CloudVeneto](https://cloudveneto.ict.unipd.it/dashboard/home/) (in this case SC4PS-PhD), one has to select in the side-bar menu the option:

**Compute → Instances**

Then select **Launch Instance**, where one must choose the mandatory options:

- **Instance Name**
- **Count**
- **Source**: choose a *Boot Source*.  
  In our case: **Image**, without creating a new volume.  
  We select *AlmaLinux10-2026-02-17*.
- **Flavor**: select compute, memory, and storage size.  
  In our case we select an option that gives 8 GB RAM (*cldarepd.large*).

Moreover, there is the optional feature to add a key pair for SSH connection.

We created an SSH key pair.

- The public key is automatically copied to `almalinux@<IP_ADDRESS>/home/almalinux/.ssh/authorized_keys` on the cloud veneto instance, while we need to copy the public and private keys onto our local `~/.ssh` repository.
- In the local repository `~/.ssh` we write on the config file

```
Host sc4ps

  HostName 10.67.20.251

  User almalinux

  IdentityFile ~/.ssh/id_sc4ps

  ProxyJump cloudveneto
```


The `Host`, in my case *sc4ps* is just a name, that will be used to do the ssh connection. The `HostName` is the IP address. In my case, since I already had an exchanged ssh key with the CloudVeneto gate, I used the `ProxyJump cloudveneto` line, where *cloudveneto* is the `Host` for *gate.cloudveneto.it*. The `User` is by default *almalinux* and `IdentityFile` needs the path to the private key (of the key pair shared with the Instance).

