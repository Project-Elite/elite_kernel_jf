<<<<<<< HEAD
/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
>>>>>>> remotes/linux2/linux-3.4.y
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
<<<<<<< HEAD
=======
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
>>>>>>> remotes/linux2/linux-3.4.y
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/bootmem.h>
#include <linux/module.h>
#include <mach/irqs.h>
#include <mach/iommu.h>
<<<<<<< HEAD
#include <mach/socinfo.h>
=======
>>>>>>> remotes/linux2/linux-3.4.y

static struct resource msm_iommu_jpegd_resources[] = {
	{
		.start = 0x07300000,
		.end   = 0x07300000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
<<<<<<< HEAD
		.start = 98,
		.end   = 98,
=======
		.start = SMMU_JPEGD_CB_SC_NON_SECURE_IRQ,
		.end   = SMMU_JPEGD_CB_SC_NON_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
<<<<<<< HEAD
		.start = 97,
		.end   = 97,
=======
		.start = SMMU_JPEGD_CB_SC_SECURE_IRQ,
		.end   = SMMU_JPEGD_CB_SC_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_vpe_resources[] = {
	{
		.start = 0x07400000,
		.end   = 0x07400000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
<<<<<<< HEAD
		.start = 84,
		.end   = 84,
=======
		.start = SMMU_VPE_CB_SC_NON_SECURE_IRQ,
		.end   = SMMU_VPE_CB_SC_NON_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
<<<<<<< HEAD
		.start = 83,
		.end   = 83,
=======
		.start = SMMU_VPE_CB_SC_SECURE_IRQ,
		.end   = SMMU_VPE_CB_SC_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_mdp0_resources[] = {
	{
		.start = 0x07500000,
		.end   = 0x07500000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
<<<<<<< HEAD
		.start = 96,
		.end   = 96,
=======
		.start = SMMU_MDP0_CB_SC_NON_SECURE_IRQ,
		.end   = SMMU_MDP0_CB_SC_NON_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
<<<<<<< HEAD
		.start = 95,
		.end   = 95,
=======
		.start = SMMU_MDP0_CB_SC_SECURE_IRQ,
		.end   = SMMU_MDP0_CB_SC_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_mdp1_resources[] = {
	{
		.start = 0x07600000,
		.end   = 0x07600000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
<<<<<<< HEAD
		.start = 94,
		.end   = 94,
=======
		.start = SMMU_MDP1_CB_SC_NON_SECURE_IRQ,
		.end   = SMMU_MDP1_CB_SC_NON_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
<<<<<<< HEAD
		.start = 93,
		.end   = 93,
=======
		.start = SMMU_MDP1_CB_SC_SECURE_IRQ,
		.end   = SMMU_MDP1_CB_SC_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_rot_resources[] = {
	{
		.start = 0x07700000,
		.end   = 0x07700000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
<<<<<<< HEAD
		.start = 92,
		.end   = 92,
=======
		.start = SMMU_ROT_CB_SC_NON_SECURE_IRQ,
		.end   = SMMU_ROT_CB_SC_NON_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
<<<<<<< HEAD
		.start = 91,
		.end   = 91,
=======
		.start = SMMU_ROT_CB_SC_SECURE_IRQ,
		.end   = SMMU_ROT_CB_SC_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_ijpeg_resources[] = {
	{
		.start = 0x07800000,
		.end   = 0x07800000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
<<<<<<< HEAD
		.start = 100,
		.end   = 100,
=======
		.start = SMMU_IJPEG_CB_SC_NON_SECURE_IRQ,
		.end   = SMMU_IJPEG_CB_SC_NON_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
<<<<<<< HEAD
		.start = 99,
		.end   = 99,
=======
		.start = SMMU_IJPEG_CB_SC_SECURE_IRQ,
		.end   = SMMU_IJPEG_CB_SC_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_vfe_resources[] = {
	{
		.start = 0x07900000,
		.end   = 0x07900000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
<<<<<<< HEAD
		.start = 86,
		.end   = 86,
=======
		.start = SMMU_VFE_CB_SC_NON_SECURE_IRQ,
		.end   = SMMU_VFE_CB_SC_NON_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
<<<<<<< HEAD
		.start = 85,
		.end   = 85,
=======
		.start = SMMU_VFE_CB_SC_SECURE_IRQ,
		.end   = SMMU_VFE_CB_SC_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_vcodec_a_resources[] = {
	{
		.start = 0x07A00000,
		.end   = 0x07A00000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
<<<<<<< HEAD
		.start = 90,
		.end   = 90,
=======
		.start = SMMU_VCODEC_A_CB_SC_NON_SECURE_IRQ,
		.end   = SMMU_VCODEC_A_CB_SC_NON_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
<<<<<<< HEAD
		.start = 89,
		.end   = 89,
=======
		.start = SMMU_VCODEC_A_CB_SC_SECURE_IRQ,
		.end   = SMMU_VCODEC_A_CB_SC_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_vcodec_b_resources[] = {
	{
		.start = 0x07B00000,
		.end   = 0x07B00000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
<<<<<<< HEAD
		.start = 88,
		.end   = 88,
=======
		.start = SMMU_VCODEC_B_CB_SC_NON_SECURE_IRQ,
		.end   = SMMU_VCODEC_B_CB_SC_NON_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
<<<<<<< HEAD
		.start = 87,
		.end   = 87,
=======
		.start = SMMU_VCODEC_B_CB_SC_SECURE_IRQ,
		.end   = SMMU_VCODEC_B_CB_SC_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_gfx3d_resources[] = {
	{
		.start = 0x07C00000,
		.end   = 0x07C00000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
<<<<<<< HEAD
		.start = 102,
		.end   = 102,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
		.start = 101,
		.end   = 101,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_gfx3d1_resources[] = {
	{
		.start = 0x07D00000,
		.end   = 0x07D00000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
		.start = 243,
		.end   = 243,
=======
		.start = SMMU_GFX3D_CB_SC_NON_SECURE_IRQ,
		.end   = SMMU_GFX3D_CB_SC_NON_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
<<<<<<< HEAD
		.start = 242,
		.end   = 242,
=======
		.start = SMMU_GFX3D_CB_SC_SECURE_IRQ,
		.end   = SMMU_GFX3D_CB_SC_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_gfx2d0_resources[] = {
	{
		.start = 0x07D00000,
		.end   = 0x07D00000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
<<<<<<< HEAD
		.start = 104,
		.end   = 104,
=======
		.start = SMMU_GFX2D0_CB_SC_NON_SECURE_IRQ,
		.end   = SMMU_GFX2D0_CB_SC_NON_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
<<<<<<< HEAD
		.start = 103,
		.end   = 103,
=======
		.start = SMMU_GFX2D0_CB_SC_SECURE_IRQ,
		.end   = SMMU_GFX2D0_CB_SC_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_gfx2d1_resources[] = {
	{
		.start = 0x07E00000,
		.end   = 0x07E00000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
<<<<<<< HEAD
		.start = 243,
		.end   = 243,
=======
		.start = SMMU_GFX2D1_CB_SC_NON_SECURE_IRQ,
		.end   = SMMU_GFX2D1_CB_SC_NON_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
<<<<<<< HEAD
		.start = 242,
		.end   = 242,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource msm_iommu_vcap_resources[] = {
	{
		.start = 0x07200000,
		.end   = 0x07200000 + SZ_1M - 1,
		.name  = "physbase",
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "nonsecure_irq",
		.start = 269,
		.end   = 269,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "secure_irq",
		.start = 268,
		.end   = 268,
=======
		.start = SMMU_GFX2D1_CB_SC_SECURE_IRQ,
		.end   = SMMU_GFX2D1_CB_SC_SECURE_IRQ,
>>>>>>> remotes/linux2/linux-3.4.y
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device msm_root_iommu_dev = {
	.name = "msm_iommu",
	.id = -1,
};

static struct msm_iommu_dev jpegd_iommu = {
	.name = "jpegd",
	.ncb = 2,
};

static struct msm_iommu_dev vpe_iommu = {
	.name = "vpe",
	.ncb = 2,
};

static struct msm_iommu_dev mdp0_iommu = {
	.name = "mdp0",
	.ncb = 2,
};

static struct msm_iommu_dev mdp1_iommu = {
	.name = "mdp1",
	.ncb = 2,
};

static struct msm_iommu_dev rot_iommu = {
	.name = "rot",
	.ncb = 2,
};

static struct msm_iommu_dev ijpeg_iommu = {
	.name = "ijpeg",
	.ncb = 2,
};

static struct msm_iommu_dev vfe_iommu = {
	.name = "vfe",
	.ncb = 2,
};

static struct msm_iommu_dev vcodec_a_iommu = {
	.name = "vcodec_a",
	.ncb = 2,
};

static struct msm_iommu_dev vcodec_b_iommu = {
	.name = "vcodec_b",
	.ncb = 2,
};

static struct msm_iommu_dev gfx3d_iommu = {
	.name = "gfx3d",
	.ncb = 3,
<<<<<<< HEAD
	.ttbr_split = 1,
};

static struct msm_iommu_dev gfx3d1_iommu = {
	.name = "gfx3d1",
	.ncb = 3,
	.ttbr_split = 1,
=======
>>>>>>> remotes/linux2/linux-3.4.y
};

static struct msm_iommu_dev gfx2d0_iommu = {
	.name = "gfx2d0",
	.ncb = 2,
<<<<<<< HEAD
	.ttbr_split = 1,
=======
>>>>>>> remotes/linux2/linux-3.4.y
};

static struct msm_iommu_dev gfx2d1_iommu = {
	.name = "gfx2d1",
	.ncb = 2,
<<<<<<< HEAD
	.ttbr_split = 1,
};

static struct msm_iommu_dev vcap_iommu = {
	.name = "vcap",
	.ncb = 2,
=======
>>>>>>> remotes/linux2/linux-3.4.y
};

static struct platform_device msm_device_iommu_jpegd = {
	.name = "msm_iommu",
	.id = 0,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
<<<<<<< HEAD
		.platform_data = &jpegd_iommu,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
	.num_resources = ARRAY_SIZE(msm_iommu_jpegd_resources),
	.resource = msm_iommu_jpegd_resources,
};

static struct platform_device msm_device_iommu_vpe = {
	.name = "msm_iommu",
	.id = 1,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
<<<<<<< HEAD
		.platform_data = &vpe_iommu,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
	.num_resources = ARRAY_SIZE(msm_iommu_vpe_resources),
	.resource = msm_iommu_vpe_resources,
};

static struct platform_device msm_device_iommu_mdp0 = {
	.name = "msm_iommu",
	.id = 2,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
<<<<<<< HEAD
		.platform_data = &mdp0_iommu,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
	.num_resources = ARRAY_SIZE(msm_iommu_mdp0_resources),
	.resource = msm_iommu_mdp0_resources,
};

static struct platform_device msm_device_iommu_mdp1 = {
	.name = "msm_iommu",
	.id = 3,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
<<<<<<< HEAD
		.platform_data = &mdp1_iommu,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
	.num_resources = ARRAY_SIZE(msm_iommu_mdp1_resources),
	.resource = msm_iommu_mdp1_resources,
};

static struct platform_device msm_device_iommu_rot = {
	.name = "msm_iommu",
	.id = 4,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
<<<<<<< HEAD
		.platform_data = &rot_iommu,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
	.num_resources = ARRAY_SIZE(msm_iommu_rot_resources),
	.resource = msm_iommu_rot_resources,
};

static struct platform_device msm_device_iommu_ijpeg = {
	.name = "msm_iommu",
	.id = 5,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
<<<<<<< HEAD
		.platform_data = &ijpeg_iommu,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
	.num_resources = ARRAY_SIZE(msm_iommu_ijpeg_resources),
	.resource = msm_iommu_ijpeg_resources,
};

static struct platform_device msm_device_iommu_vfe = {
	.name = "msm_iommu",
	.id = 6,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
<<<<<<< HEAD
		.platform_data = &vfe_iommu,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
	.num_resources = ARRAY_SIZE(msm_iommu_vfe_resources),
	.resource = msm_iommu_vfe_resources,
};

static struct platform_device msm_device_iommu_vcodec_a = {
	.name = "msm_iommu",
	.id = 7,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
<<<<<<< HEAD
		.platform_data = &vcodec_a_iommu,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
	.num_resources = ARRAY_SIZE(msm_iommu_vcodec_a_resources),
	.resource = msm_iommu_vcodec_a_resources,
};

static struct platform_device msm_device_iommu_vcodec_b = {
	.name = "msm_iommu",
	.id = 8,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
<<<<<<< HEAD
		.platform_data = &vcodec_b_iommu,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
	.num_resources = ARRAY_SIZE(msm_iommu_vcodec_b_resources),
	.resource = msm_iommu_vcodec_b_resources,
};

static struct platform_device msm_device_iommu_gfx3d = {
	.name = "msm_iommu",
	.id = 9,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
<<<<<<< HEAD
		.platform_data = &gfx3d_iommu,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
	.num_resources = ARRAY_SIZE(msm_iommu_gfx3d_resources),
	.resource = msm_iommu_gfx3d_resources,
};

<<<<<<< HEAD
static struct platform_device msm_device_iommu_gfx3d1 = {
	.name = "msm_iommu",
	.id = 10,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
		.platform_data = &gfx3d1_iommu,
	},
	.num_resources = ARRAY_SIZE(msm_iommu_gfx3d1_resources),
	.resource = msm_iommu_gfx3d1_resources,
};

=======
>>>>>>> remotes/linux2/linux-3.4.y
static struct platform_device msm_device_iommu_gfx2d0 = {
	.name = "msm_iommu",
	.id = 10,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
<<<<<<< HEAD
		.platform_data = &gfx2d0_iommu,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
	.num_resources = ARRAY_SIZE(msm_iommu_gfx2d0_resources),
	.resource = msm_iommu_gfx2d0_resources,
};

<<<<<<< HEAD
static struct platform_device msm_device_iommu_gfx2d1 = {
=======
struct platform_device msm_device_iommu_gfx2d1 = {
>>>>>>> remotes/linux2/linux-3.4.y
	.name = "msm_iommu",
	.id = 11,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
<<<<<<< HEAD
		.platform_data = &gfx2d1_iommu,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
	.num_resources = ARRAY_SIZE(msm_iommu_gfx2d1_resources),
	.resource = msm_iommu_gfx2d1_resources,
};

<<<<<<< HEAD
static struct platform_device msm_device_iommu_vcap = {
	.name = "msm_iommu",
	.id = 11,
	.dev = {
		.parent = &msm_root_iommu_dev.dev,
		.platform_data = &vcap_iommu,
	},
	.num_resources = ARRAY_SIZE(msm_iommu_vcap_resources),
	.resource = msm_iommu_vcap_resources,
};

=======
>>>>>>> remotes/linux2/linux-3.4.y
static struct msm_iommu_ctx_dev jpegd_src_ctx = {
	.name = "jpegd_src",
	.num = 0,
	.mids = {0, -1}
};

static struct msm_iommu_ctx_dev jpegd_dst_ctx = {
	.name = "jpegd_dst",
	.num = 1,
	.mids = {1, -1}
};

static struct msm_iommu_ctx_dev vpe_src_ctx = {
	.name = "vpe_src",
	.num = 0,
	.mids = {0, -1}
};

static struct msm_iommu_ctx_dev vpe_dst_ctx = {
	.name = "vpe_dst",
	.num = 1,
	.mids = {1, -1}
};

<<<<<<< HEAD
static struct msm_iommu_ctx_dev mdp_port0_cb0_ctx = {
	.name = "mdp_port0_cb0",
=======
static struct msm_iommu_ctx_dev mdp_vg1_ctx = {
	.name = "mdp_vg1",
>>>>>>> remotes/linux2/linux-3.4.y
	.num = 0,
	.mids = {0, 2, -1}
};

<<<<<<< HEAD
static struct msm_iommu_ctx_dev mdp_port0_cb1_ctx = {
	.name = "mdp_port0_cb1",
=======
static struct msm_iommu_ctx_dev mdp_rgb1_ctx = {
	.name = "mdp_rgb1",
>>>>>>> remotes/linux2/linux-3.4.y
	.num = 1,
	.mids = {1, 3, 4, 5, 6, 7, 8, 9, 10, -1}
};

<<<<<<< HEAD
static struct msm_iommu_ctx_dev mdp_port1_cb0_ctx = {
	.name = "mdp_port1_cb0",
=======
static struct msm_iommu_ctx_dev mdp_vg2_ctx = {
	.name = "mdp_vg2",
>>>>>>> remotes/linux2/linux-3.4.y
	.num = 0,
	.mids = {0, 2, -1}
};

<<<<<<< HEAD
static struct msm_iommu_ctx_dev mdp_port1_cb1_ctx = {
	.name = "mdp_port1_cb1",
=======
static struct msm_iommu_ctx_dev mdp_rgb2_ctx = {
	.name = "mdp_rgb2",
>>>>>>> remotes/linux2/linux-3.4.y
	.num = 1,
	.mids = {1, 3, 4, 5, 6, 7, 8, 9, 10, -1}
};

static struct msm_iommu_ctx_dev rot_src_ctx = {
	.name = "rot_src",
	.num = 0,
	.mids = {0, -1}
};

static struct msm_iommu_ctx_dev rot_dst_ctx = {
	.name = "rot_dst",
	.num = 1,
	.mids = {1, -1}
};

static struct msm_iommu_ctx_dev ijpeg_src_ctx = {
	.name = "ijpeg_src",
	.num = 0,
	.mids = {0, -1}
};

static struct msm_iommu_ctx_dev ijpeg_dst_ctx = {
	.name = "ijpeg_dst",
	.num = 1,
	.mids = {1, -1}
};

static struct msm_iommu_ctx_dev vfe_imgwr_ctx = {
	.name = "vfe_imgwr",
	.num = 0,
	.mids = {2, 3, 4, 5, 6, 7, 8, -1}
};

static struct msm_iommu_ctx_dev vfe_misc_ctx = {
	.name = "vfe_misc",
	.num = 1,
	.mids = {0, 1, 9, -1}
};

static struct msm_iommu_ctx_dev vcodec_a_stream_ctx = {
	.name = "vcodec_a_stream",
	.num = 0,
	.mids = {2, 5, -1}
};

static struct msm_iommu_ctx_dev vcodec_a_mm1_ctx = {
	.name = "vcodec_a_mm1",
	.num = 1,
	.mids = {0, 1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1}
};

static struct msm_iommu_ctx_dev vcodec_b_mm2_ctx = {
	.name = "vcodec_b_mm2",
	.num = 0,
	.mids = {0, 1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1}
};

static struct msm_iommu_ctx_dev gfx3d_user_ctx = {
	.name = "gfx3d_user",
	.num = 0,
	.mids = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1}
};

static struct msm_iommu_ctx_dev gfx3d_priv_ctx = {
	.name = "gfx3d_priv",
	.num = 1,
	.mids = {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		 31, -1}
};

<<<<<<< HEAD
static struct msm_iommu_ctx_dev gfx3d1_user_ctx = {
	.name = "gfx3d1_user",
	.num = 0,
	.mids = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1}
};

static struct msm_iommu_ctx_dev gfx3d1_priv_ctx = {
	.name = "gfx3d1_priv",
	.num = 1,
	.mids = {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
		 31, -1}
};

=======
>>>>>>> remotes/linux2/linux-3.4.y
static struct msm_iommu_ctx_dev gfx2d0_2d0_ctx = {
	.name = "gfx2d0_2d0",
	.num = 0,
	.mids = {0, 1, 2, 3, 4, 5, 6, 7, -1}
};

static struct msm_iommu_ctx_dev gfx2d1_2d1_ctx = {
	.name = "gfx2d1_2d1",
	.num = 0,
	.mids = {0, 1, 2, 3, 4, 5, 6, 7, -1}
};

<<<<<<< HEAD
static struct msm_iommu_ctx_dev vcap_vc_ctx = {
	.name = "vcap_vc",
	.num = 0,
	.mids = {0, -1}
};

static struct msm_iommu_ctx_dev vcap_vp_ctx = {
	.name = "vcap_vp",
	.num = 1,
	.mids = {1, -1}
};

=======
>>>>>>> remotes/linux2/linux-3.4.y
static struct platform_device msm_device_jpegd_src_ctx = {
	.name = "msm_iommu_ctx",
	.id = 0,
	.dev = {
		.parent = &msm_device_iommu_jpegd.dev,
<<<<<<< HEAD
		.platform_data = &jpegd_src_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_jpegd_dst_ctx = {
	.name = "msm_iommu_ctx",
	.id = 1,
	.dev = {
		.parent = &msm_device_iommu_jpegd.dev,
<<<<<<< HEAD
		.platform_data = &jpegd_dst_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_vpe_src_ctx = {
	.name = "msm_iommu_ctx",
	.id = 2,
	.dev = {
		.parent = &msm_device_iommu_vpe.dev,
<<<<<<< HEAD
		.platform_data = &vpe_src_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_vpe_dst_ctx = {
	.name = "msm_iommu_ctx",
	.id = 3,
	.dev = {
		.parent = &msm_device_iommu_vpe.dev,
<<<<<<< HEAD
		.platform_data = &vpe_dst_ctx,
	},
};

static struct platform_device msm_device_mdp_port0_cb0_ctx = {
=======
	},
};

static struct platform_device msm_device_mdp_vg1_ctx = {
>>>>>>> remotes/linux2/linux-3.4.y
	.name = "msm_iommu_ctx",
	.id = 4,
	.dev = {
		.parent = &msm_device_iommu_mdp0.dev,
<<<<<<< HEAD
		.platform_data = &mdp_port0_cb0_ctx,
	},
};

static struct platform_device msm_device_mdp_port0_cb1_ctx = {
=======
	},
};

static struct platform_device msm_device_mdp_rgb1_ctx = {
>>>>>>> remotes/linux2/linux-3.4.y
	.name = "msm_iommu_ctx",
	.id = 5,
	.dev = {
		.parent = &msm_device_iommu_mdp0.dev,
<<<<<<< HEAD
		.platform_data = &mdp_port0_cb1_ctx,
	},
};

static struct platform_device msm_device_mdp_port1_cb0_ctx = {
=======
	},
};

static struct platform_device msm_device_mdp_vg2_ctx = {
>>>>>>> remotes/linux2/linux-3.4.y
	.name = "msm_iommu_ctx",
	.id = 6,
	.dev = {
		.parent = &msm_device_iommu_mdp1.dev,
<<<<<<< HEAD
		.platform_data = &mdp_port1_cb0_ctx,
	},
};

static struct platform_device msm_device_mdp_port1_cb1_ctx = {
=======
	},
};

static struct platform_device msm_device_mdp_rgb2_ctx = {
>>>>>>> remotes/linux2/linux-3.4.y
	.name = "msm_iommu_ctx",
	.id = 7,
	.dev = {
		.parent = &msm_device_iommu_mdp1.dev,
<<<<<<< HEAD
		.platform_data = &mdp_port1_cb1_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_rot_src_ctx = {
	.name = "msm_iommu_ctx",
	.id = 8,
	.dev = {
		.parent = &msm_device_iommu_rot.dev,
<<<<<<< HEAD
		.platform_data = &rot_src_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_rot_dst_ctx = {
	.name = "msm_iommu_ctx",
	.id = 9,
	.dev = {
		.parent = &msm_device_iommu_rot.dev,
<<<<<<< HEAD
		.platform_data = &rot_dst_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_ijpeg_src_ctx = {
	.name = "msm_iommu_ctx",
	.id = 10,
	.dev = {
		.parent = &msm_device_iommu_ijpeg.dev,
<<<<<<< HEAD
		.platform_data = &ijpeg_src_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_ijpeg_dst_ctx = {
	.name = "msm_iommu_ctx",
	.id = 11,
	.dev = {
		.parent = &msm_device_iommu_ijpeg.dev,
<<<<<<< HEAD
		.platform_data = &ijpeg_dst_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_vfe_imgwr_ctx = {
	.name = "msm_iommu_ctx",
	.id = 12,
	.dev = {
		.parent = &msm_device_iommu_vfe.dev,
<<<<<<< HEAD
		.platform_data = &vfe_imgwr_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_vfe_misc_ctx = {
	.name = "msm_iommu_ctx",
	.id = 13,
	.dev = {
		.parent = &msm_device_iommu_vfe.dev,
<<<<<<< HEAD
		.platform_data = &vfe_misc_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_vcodec_a_stream_ctx = {
	.name = "msm_iommu_ctx",
	.id = 14,
	.dev = {
		.parent = &msm_device_iommu_vcodec_a.dev,
<<<<<<< HEAD
		.platform_data = &vcodec_a_stream_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_vcodec_a_mm1_ctx = {
	.name = "msm_iommu_ctx",
	.id = 15,
	.dev = {
		.parent = &msm_device_iommu_vcodec_a.dev,
<<<<<<< HEAD
		.platform_data = &vcodec_a_mm1_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_vcodec_b_mm2_ctx = {
	.name = "msm_iommu_ctx",
	.id = 16,
	.dev = {
		.parent = &msm_device_iommu_vcodec_b.dev,
<<<<<<< HEAD
		.platform_data = &vcodec_b_mm2_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_gfx3d_user_ctx = {
	.name = "msm_iommu_ctx",
	.id = 17,
	.dev = {
		.parent = &msm_device_iommu_gfx3d.dev,
<<<<<<< HEAD
		.platform_data = &gfx3d_user_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_gfx3d_priv_ctx = {
	.name = "msm_iommu_ctx",
	.id = 18,
	.dev = {
		.parent = &msm_device_iommu_gfx3d.dev,
<<<<<<< HEAD
		.platform_data = &gfx3d_priv_ctx,
	},
};

static struct platform_device msm_device_gfx3d1_user_ctx = {
	.name = "msm_iommu_ctx",
	.id = 19,
	.dev = {
		.parent = &msm_device_iommu_gfx3d1.dev,
		.platform_data = &gfx3d1_user_ctx,
	},
};

static struct platform_device msm_device_gfx3d1_priv_ctx = {
	.name = "msm_iommu_ctx",
	.id = 20,
	.dev = {
		.parent = &msm_device_iommu_gfx3d1.dev,
		.platform_data = &gfx3d1_priv_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_gfx2d0_2d0_ctx = {
	.name = "msm_iommu_ctx",
	.id = 19,
	.dev = {
		.parent = &msm_device_iommu_gfx2d0.dev,
<<<<<<< HEAD
		.platform_data = &gfx2d0_2d0_ctx,
=======
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static struct platform_device msm_device_gfx2d1_2d1_ctx = {
	.name = "msm_iommu_ctx",
	.id = 20,
	.dev = {
		.parent = &msm_device_iommu_gfx2d1.dev,
<<<<<<< HEAD
		.platform_data = &gfx2d1_2d1_ctx,
	},
};

static struct platform_device msm_device_vcap_vc_ctx = {
	.name = "msm_iommu_ctx",
	.id = 21,
	.dev = {
		.parent = &msm_device_iommu_vcap.dev,
		.platform_data = &vcap_vc_ctx,
	},
};

static struct platform_device msm_device_vcap_vp_ctx = {
	.name = "msm_iommu_ctx",
	.id = 22,
	.dev = {
		.parent = &msm_device_iommu_vcap.dev,
		.platform_data = &vcap_vp_ctx,
	},
};

static struct platform_device *msm_iommu_common_devs[] = {
=======
	},
};

static struct platform_device *msm_iommu_devs[] = {
	&msm_device_iommu_jpegd,
>>>>>>> remotes/linux2/linux-3.4.y
	&msm_device_iommu_vpe,
	&msm_device_iommu_mdp0,
	&msm_device_iommu_mdp1,
	&msm_device_iommu_rot,
	&msm_device_iommu_ijpeg,
	&msm_device_iommu_vfe,
	&msm_device_iommu_vcodec_a,
	&msm_device_iommu_vcodec_b,
	&msm_device_iommu_gfx3d,
<<<<<<< HEAD
};

static struct platform_device *msm_iommu_gfx2d_devs[] = {
=======
>>>>>>> remotes/linux2/linux-3.4.y
	&msm_device_iommu_gfx2d0,
	&msm_device_iommu_gfx2d1,
};

<<<<<<< HEAD
static struct platform_device *msm_iommu_adreno3xx_gfx_devs[] = {
	&msm_device_iommu_gfx3d1,
};

static struct platform_device *msm_iommu_vcap_devs[] = {
	&msm_device_iommu_vcap,
};

static struct platform_device *msm_iommu_jpegd_devs[] = {
	&msm_device_iommu_jpegd,
};

static struct platform_device *msm_iommu_common_ctx_devs[] = {
	&msm_device_vpe_src_ctx,
	&msm_device_vpe_dst_ctx,
	&msm_device_mdp_port0_cb0_ctx,
	&msm_device_mdp_port0_cb1_ctx,
	&msm_device_mdp_port1_cb0_ctx,
	&msm_device_mdp_port1_cb1_ctx,
=======
static struct msm_iommu_dev *msm_iommu_data[] = {
	&jpegd_iommu,
	&vpe_iommu,
	&mdp0_iommu,
	&mdp1_iommu,
	&rot_iommu,
	&ijpeg_iommu,
	&vfe_iommu,
	&vcodec_a_iommu,
	&vcodec_b_iommu,
	&gfx3d_iommu,
	&gfx2d0_iommu,
	&gfx2d1_iommu,
};

static struct platform_device *msm_iommu_ctx_devs[] = {
	&msm_device_jpegd_src_ctx,
	&msm_device_jpegd_dst_ctx,
	&msm_device_vpe_src_ctx,
	&msm_device_vpe_dst_ctx,
	&msm_device_mdp_vg1_ctx,
	&msm_device_mdp_rgb1_ctx,
	&msm_device_mdp_vg2_ctx,
	&msm_device_mdp_rgb2_ctx,
>>>>>>> remotes/linux2/linux-3.4.y
	&msm_device_rot_src_ctx,
	&msm_device_rot_dst_ctx,
	&msm_device_ijpeg_src_ctx,
	&msm_device_ijpeg_dst_ctx,
	&msm_device_vfe_imgwr_ctx,
	&msm_device_vfe_misc_ctx,
	&msm_device_vcodec_a_stream_ctx,
	&msm_device_vcodec_a_mm1_ctx,
	&msm_device_vcodec_b_mm2_ctx,
	&msm_device_gfx3d_user_ctx,
	&msm_device_gfx3d_priv_ctx,
<<<<<<< HEAD
};

static struct platform_device *msm_iommu_gfx2d_ctx_devs[] = {
=======
>>>>>>> remotes/linux2/linux-3.4.y
	&msm_device_gfx2d0_2d0_ctx,
	&msm_device_gfx2d1_2d1_ctx,
};

<<<<<<< HEAD
static struct platform_device *msm_iommu_adreno3xx_ctx_devs[] = {
	&msm_device_gfx3d1_user_ctx,
	&msm_device_gfx3d1_priv_ctx,
};

static struct platform_device *msm_iommu_vcap_ctx_devs[] = {
	&msm_device_vcap_vc_ctx,
	&msm_device_vcap_vp_ctx,
};

static struct platform_device *msm_iommu_jpegd_ctx_devs[] = {
	&msm_device_jpegd_src_ctx,
	&msm_device_jpegd_dst_ctx,
};

static int __init iommu_init(void)
{
	int ret;
	if (!msm_soc_version_supports_iommu_v1()) {
		pr_err("IOMMU v1 is not supported on this SoC version.\n");
		return -ENODEV;
	}
=======
static struct msm_iommu_ctx_dev *msm_iommu_ctx_data[] = {
	&jpegd_src_ctx,
	&jpegd_dst_ctx,
	&vpe_src_ctx,
	&vpe_dst_ctx,
	&mdp_vg1_ctx,
	&mdp_rgb1_ctx,
	&mdp_vg2_ctx,
	&mdp_rgb2_ctx,
	&rot_src_ctx,
	&rot_dst_ctx,
	&ijpeg_src_ctx,
	&ijpeg_dst_ctx,
	&vfe_imgwr_ctx,
	&vfe_misc_ctx,
	&vcodec_a_stream_ctx,
	&vcodec_a_mm1_ctx,
	&vcodec_b_mm2_ctx,
	&gfx3d_user_ctx,
	&gfx3d_priv_ctx,
	&gfx2d0_2d0_ctx,
	&gfx2d1_2d1_ctx,
};

static int __init msm8x60_iommu_init(void)
{
	int ret, i;
>>>>>>> remotes/linux2/linux-3.4.y

	ret = platform_device_register(&msm_root_iommu_dev);
	if (ret != 0) {
		pr_err("Failed to register root IOMMU device!\n");
		goto failure;
	}

<<<<<<< HEAD
	/* Initialize common devs */
	platform_add_devices(msm_iommu_common_devs,
				ARRAY_SIZE(msm_iommu_common_devs));

	/* Initialize soc-specific devs */
	if (cpu_is_msm8x60() || cpu_is_msm8960()) {
		platform_add_devices(msm_iommu_jpegd_devs,
				ARRAY_SIZE(msm_iommu_jpegd_devs));
		platform_add_devices(msm_iommu_gfx2d_devs,
				ARRAY_SIZE(msm_iommu_gfx2d_devs));
	}

	if (soc_class_is_apq8064() || cpu_is_msm8960ab()) {
		platform_add_devices(msm_iommu_jpegd_devs,
				ARRAY_SIZE(msm_iommu_jpegd_devs));
		platform_add_devices(msm_iommu_adreno3xx_gfx_devs,
				ARRAY_SIZE(msm_iommu_adreno3xx_gfx_devs));
	}
	if (soc_class_is_apq8064())
		platform_add_devices(msm_iommu_vcap_devs,
				ARRAY_SIZE(msm_iommu_vcap_devs));

	/* Initialize common ctx_devs */
	ret = platform_add_devices(msm_iommu_common_ctx_devs,
				ARRAY_SIZE(msm_iommu_common_ctx_devs));

	/* Initialize soc-specific ctx_devs */
	if (cpu_is_msm8x60() || cpu_is_msm8960()) {
		platform_add_devices(msm_iommu_jpegd_ctx_devs,
				ARRAY_SIZE(msm_iommu_jpegd_ctx_devs));
		platform_add_devices(msm_iommu_gfx2d_ctx_devs,
				ARRAY_SIZE(msm_iommu_gfx2d_ctx_devs));
	}

	if (soc_class_is_apq8064() || cpu_is_msm8960ab()) {
		platform_add_devices(msm_iommu_jpegd_ctx_devs,
				ARRAY_SIZE(msm_iommu_jpegd_ctx_devs));

		platform_add_devices(msm_iommu_adreno3xx_ctx_devs,
				ARRAY_SIZE(msm_iommu_adreno3xx_ctx_devs));
	}
	if (soc_class_is_apq8064())
		platform_add_devices(msm_iommu_vcap_ctx_devs,
			ARRAY_SIZE(msm_iommu_vcap_ctx_devs));

	return 0;

=======
	for (i = 0; i < ARRAY_SIZE(msm_iommu_devs); i++) {
		ret = platform_device_add_data(msm_iommu_devs[i],
					       msm_iommu_data[i],
					       sizeof(struct msm_iommu_dev));
		if (ret != 0) {
			pr_err("platform_device_add_data failed, "
			       "i = %d\n", i);
			goto failure_unwind;
		}

		ret = platform_device_register(msm_iommu_devs[i]);

		if (ret != 0) {
			pr_err("platform_device_register iommu failed, "
			       "i = %d\n", i);
			goto failure_unwind;
		}
	}

	for (i = 0; i < ARRAY_SIZE(msm_iommu_ctx_devs); i++) {
		ret = platform_device_add_data(msm_iommu_ctx_devs[i],
					       msm_iommu_ctx_data[i],
					       sizeof(*msm_iommu_ctx_devs[i]));
		if (ret != 0) {
			pr_err("platform_device_add_data iommu failed, "
			       "i = %d\n", i);
			goto failure_unwind2;
		}

		ret = platform_device_register(msm_iommu_ctx_devs[i]);
		if (ret != 0) {
			pr_err("platform_device_register ctx failed, "
			       "i = %d\n", i);
			goto failure_unwind2;
		}
	}
	return 0;

failure_unwind2:
	while (--i >= 0)
		platform_device_unregister(msm_iommu_ctx_devs[i]);
failure_unwind:
	while (--i >= 0)
		platform_device_unregister(msm_iommu_devs[i]);

	platform_device_unregister(&msm_root_iommu_dev);
>>>>>>> remotes/linux2/linux-3.4.y
failure:
	return ret;
}

<<<<<<< HEAD
static void __exit iommu_exit(void)
{
	int i;

	/* Common ctx_devs */
	for (i = 0; i < ARRAY_SIZE(msm_iommu_common_ctx_devs); i++)
		platform_device_unregister(msm_iommu_common_ctx_devs[i]);

	/* Common devs. */
	for (i = 0; i < ARRAY_SIZE(msm_iommu_common_devs); ++i)
		platform_device_unregister(msm_iommu_common_devs[i]);

	if (cpu_is_msm8x60() || cpu_is_msm8960()) {
		for (i = 0; i < ARRAY_SIZE(msm_iommu_gfx2d_ctx_devs); i++)
			platform_device_unregister(msm_iommu_gfx2d_ctx_devs[i]);

		for (i = 0; i < ARRAY_SIZE(msm_iommu_jpegd_ctx_devs); i++)
			platform_device_unregister(msm_iommu_jpegd_ctx_devs[i]);

		for (i = 0; i < ARRAY_SIZE(msm_iommu_gfx2d_devs); i++)
			platform_device_unregister(msm_iommu_gfx2d_devs[i]);

		for (i = 0; i < ARRAY_SIZE(msm_iommu_jpegd_devs); i++)
			platform_device_unregister(msm_iommu_jpegd_devs[i]);
	}
	if (soc_class_is_apq8064()) {
		for (i = 0; i < ARRAY_SIZE(msm_iommu_vcap_ctx_devs); i++)
			platform_device_unregister(msm_iommu_vcap_ctx_devs[i]);
	}

	if (soc_class_is_apq8064() || cpu_is_msm8960ab()) {
		for (i = 0; i < ARRAY_SIZE(msm_iommu_adreno3xx_ctx_devs);
			   i++)
			platform_device_unregister(
				msm_iommu_adreno3xx_ctx_devs[i]);

		for (i = 0; i < ARRAY_SIZE(msm_iommu_jpegd_ctx_devs);
			   i++)
			platform_device_unregister(
				msm_iommu_jpegd_ctx_devs[i]);

		if (soc_class_is_apq8064()) {
			for (i = 0; i < ARRAY_SIZE(msm_iommu_vcap_devs);
			   i++)
				platform_device_unregister(
				msm_iommu_vcap_devs[i]);
		}

		for (i = 0; i < ARRAY_SIZE(msm_iommu_adreno3xx_gfx_devs);
			   i++)
			platform_device_unregister(
				msm_iommu_adreno3xx_gfx_devs[i]);

		for (i = 0; i < ARRAY_SIZE(msm_iommu_jpegd_devs); i++)
			platform_device_unregister(msm_iommu_jpegd_devs[i]);
	}
=======
static void __exit msm8x60_iommu_exit(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(msm_iommu_ctx_devs); i++)
		platform_device_unregister(msm_iommu_ctx_devs[i]);

	for (i = 0; i < ARRAY_SIZE(msm_iommu_devs); ++i)
		platform_device_unregister(msm_iommu_devs[i]);
>>>>>>> remotes/linux2/linux-3.4.y

	platform_device_unregister(&msm_root_iommu_dev);
}

<<<<<<< HEAD
subsys_initcall(iommu_init);
module_exit(iommu_exit);
=======
subsys_initcall(msm8x60_iommu_init);
module_exit(msm8x60_iommu_exit);
>>>>>>> remotes/linux2/linux-3.4.y

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Stepan Moskovchenko <stepanm@codeaurora.org>");
