#Homework01: Guide to setup my Linux machine in CLoudVeneto for compiling and running c codes

#CloudVeneto Setup
Once successfully accessed a project in [CloudeVeneto](https://cloudveneto.ict.unipd.it/dashboard/home/), in this case SC4PS-PhD, one has to select in the side-bar menu the option Compute -> Instances. 
Then one has to select *Launch Instance*, where one has to select the mandatory oprions:
-item *Instance Name*.
-item *Count*.
-item *Source*: where one should choos a *Boot Source*, in our case *Image*, without creating a new volume. In our case we select *AlmaLinux10-2026-02-17.
-item *Flavor*, selecting the sizing for the compute, memory and storage capacity of the instance. In our case we select an option that gives us 8 GB of RAM (*cldarepd.large*).
Moreover, there is the optional feature to add a key pair for ssh connection. We created an SSH key-pair. The public key is authomaticcally copied onto CloudVeneto/user/home/.ssh, while we need to copy the public and private keys onto our local ~/.ssh repository.

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

- The public key is automatically copied to: CloudVeneto/user/home/.ssh while we need to copy the public and private keys onto our local ~/.ssh repository.
