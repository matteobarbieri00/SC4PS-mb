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
